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

// For name conflict
ssize_t socket_recv(int __fd, void* __buf, size_t __n, int __flags);

Net_Client::Net_Client(unsigned int user, std::string passwd, unsigned int flag)
{
	this->flag = flag | CLIENT;
	int error_code;
	if (TCP & flag)
	{
		std::cout << "Net_Client:enabling TCP mode" << std::endl;

		Addr targ;
		int conn_socket;

		// socket();
		conn_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (conn_socket == -1)
		{
			std::cout << "Net_Client:TCP:socket:error, " << errno << std::endl;
			return;
		}

		// connect();
		targ.addr_in.sin_family = AF_INET;
		targ.addr_in.sin_addr.s_addr = inet_addr(SERVER_IP);
		targ.addr_in.sin_port = htons(TCP_PORT);
		error_code = connect(conn_socket, &(targ.addr), sizeof(sockaddr));
		if (error_code == -1)
		{
			std::cout << "Net_Client:TCP:connect:error " << errno << std::endl;
			close(conn_socket);
			return;
		}

		// create a thread to recv();
		std::thread t(&Net_Client::TCP_RECV_thread, this, conn_socket);
		t.detach();

		// insert {0, _socket} to TCP_T,
		// for the use of send.
		(this->TCP_T_lock).lock();
		(this->TCP_T).insert(type_TCP_T::value_type(0, conn_socket));
		(this->TCP_T_lock).unlock();

		// login
		if (passwd.length() >= 60)
		{
			std::cout << "Net_Client:TCP:login:password is too long." << std::endl;
			close(conn_socket);
			return;
		}

		char* buf = (char*) malloc(passwd.length() + 5);
		memcpy(buf, &user, 4);
		memcpy(buf + 4, passwd.c_str(), passwd.length() + 1);
		std::unique_ptr<char[], void(*)(void*)> p(buf, &free);
		Msg m(std::move(p), passwd.length() + 5, 0);
		buf = nullptr;
		send(std::move(m), TCP);
	}
}

void Net_Client::TCP_RECV_thread(int conn_socket)
{
	int n = 0;
	char* buf;
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
				std::cout << "Net_Client:TCP:recv:closed." << std::endl;
			}
			else
			{
				std::cout << "Net_Client:TCP:recv:error." << std::endl;
			}
			close(conn_socket);
			free(buf);
			return;
		}
		else
		{
			// shrink to fit
			buf = (char*)realloc(buf, n);

			// create a new Msg
			std::unique_ptr<char[], void(*)(void*)> p(buf, &free);
			Msg m(std::move(p), n, 0);
			buf = nullptr;

			// add it to queue
			(this->recv_queue_lock).lock();
			(this->recv_queue).push(std::move(m));
			(this->recv_queue_lock).unlock();
		}
	}
}