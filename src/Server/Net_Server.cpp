#include "Network.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <memory>

#ifdef LINUX
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <arpa/inet.h>
#endif

#define SERVER 0U
#define CLIENT 4U

#define min(a, b) ((a < b)?a:b)

// For name conflict
ssize_t socket_recv(int __fd, void* __buf, size_t __n, int __flags);

Net_Server::Net_Server(type_func_login login, const unsigned int flag)
{
	this->flag = flag | SERVER;
	int error_code;
	if (TCP & flag)
	{
		std::cout << "Net_Server:enabling TCP mode" << std::endl;

		Addr scr;
		int acpt_socket;

		// socket();
		acpt_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (acpt_socket == -1)
		{
			std::cout << "Net_Server:TCP:socket:error, " << errno << std::endl;
			return;
		}

		// bind();
		scr.addr_in.sin_family = AF_INET;
		scr.addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
		scr.addr_in.sin_port = htons(TCP_PORT);
		error_code = bind(acpt_socket, &(scr.addr), sizeof(sockaddr));
		if (error_code == -1)
		{
			std::cout << "Net_Server:TCP:bind:error, " << errno << std::endl;
			close(acpt_socket);
			return;
		}

		//listen();
		error_code = listen(acpt_socket, 50);
		if (error_code == -1)
		{
			std::cout << "Net_Server:TCP:listen:error, " << errno << std::endl;
			close(acpt_socket);
			return;
		}

		// create a thread to accept();
		std::thread t(&Net_Server::TCP_ACPT_thread, this, acpt_socket, login);
		t.detach();

		// insert {0, acpt_socket} to TCP_T,
		// for the use of destructor
		(this->TCP_T_lock).lock();
		(this->TCP_T).insert(type_TCP_T::value_type(0, acpt_socket));
		(this->TCP_T_lock).unlock();
	}
	if (UDP & flag)
	{

	}
}

void Net_Server::TCP_ACPT_thread(int acpt_socket, type_func_login login)
{
	int conn_socket;
	while (true)
	{
		// accept();
		conn_socket = accept(acpt_socket, NULL, 0);
		if (conn_socket == -1)
		{
			std::cout << "Net_Server:TCP:accept:error, " << errno << std::endl;
		}
		else
		{
			std::thread t(&Net_Server::TCP_RECV_thread, this, conn_socket, login);
			t.detach();
			// create a thread to recv();
		}
	}
}
void Net_Server::TCP_RECV_thread(int conn_socket, type_func_login login)
{
	char* buf = (char*)malloc(64);
	int n = 0;

	n = socket_recv(conn_socket, buf, 64, 0);
	buf[min(n, 63)] = '\0';

	// check the format of the received message
	if (n <= 4)
	{
		std::cout << "Net_Server:TCP:login:invalid login." << std::endl;
		close(conn_socket);
		free(buf);
		return;
	}

	unsigned int user = *(unsigned int*) &(buf[0]);
	std::string passwd(&buf[4], n - 4);

	// check the password
	if (!login(user, passwd))
	{
		std::cout << "Net_Server:TCP:login:wrong passwd, user " << user << std::endl;
		close(conn_socket);
		free(buf);
		return;
	}
	// login successfully
	std::cout << "Net_Server:TCP:login:login successful, user " << user << std::endl;
	free(buf);

	// insert {user, conn_socket} to TCP_T,
	// for use of send.
	(this->TCP_T_lock).lock();
	if ((this->TCP_T).left.count(user) > 0)
	{
		// this user has already exists
		close((this->TCP_T).left.find(user)->second);
		(this->TCP_T).left.erase(user);
		(this->TCP_T).insert(type_TCP_T::value_type(user, conn_socket));
	}
	else
	{
		// user doesn't exits
		(this->TCP_T).insert(type_TCP_T::value_type(user, conn_socket));
	}
	(this->TCP_T_lock).unlock();

	// recv();
	while (true)
	{
		// malloc the space
		buf = (char*)malloc(4096);
		// recv();
		n = socket_recv(conn_socket, buf, 4096, 0);
		if (n <= 0)
		{
			if (n == 0)
			{
				// TCP connection has closed
				std::cout << "Net_Server:TCP:recv:closed, user " << user << std::endl;
			}
			else
			{
				std::cout << "Net_Server:TCP:recv:error, user " << user << ", " << errno << std::endl;
			}
			close(conn_socket);
			free(buf);
			// delete {user, conn_socket} in TCP_T if socket and user are paired
			(this->TCP_T_lock).lock();
			if ((this->TCP_T).left.find(user)->second == conn_socket)
			{
				(this->TCP_T).erase(type_TCP_T::value_type(user, conn_socket));
			}
			(this->TCP_T_lock).unlock();
			// exit thread
			return;
		}
		else
		{
			// shrink to fit
			buf = (char*)realloc(buf, n);

			// create a new Msg
			std::unique_ptr<char[], void(*)(void*)> p(buf, free);
			Msg m(std::move(p), n, user);
			buf = nullptr;

			// add it to queue
			(this->recv_queue_lock).lock();
			(this->recv_queue).push(std::move(m));
			(this->recv_queue_lock).unlock();
		}
	}
}