#include "Net_Server.hpp"
#ifdef WINDOWS
	#include <Winsock2.h>
	#include <Ws2tcpip.h>
#endif
#ifdef LINUX
	#include <arpa/inet.h>
#endif
#include <iostream>
#include <thread>

#define SERVER 0U
#define CLIENT 4U

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
	Msg log_m(this->TCP_recv_helper(conn_socket));
	if (log_m._buf.get() == nullptr)
	{
		std::cout << "Net_Server:TCP:login:invalid login." << std::endl;
		close(conn_socket);
		return;
	}

	unsigned int user = *(unsigned int*) log_m._buf.get();
	std::string passwd(log_m._buf.get() + 4, log_m._size - 4);
	if (!login(user, passwd))
	{
		std::cout << "Net_Server:TCP:login:wrong passwd, user " << user << std::endl;
		close(conn_socket);
		return;
	}

    std::cout << "Net_Server:TCP:login:login successful, user " << user << std::endl;

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
        Msg m(this->TCP_recv_helper(conn_socket));
        if(m._buf.get() == nullptr){
            std::cout << "Net_Server:TCP:recv:error, user " << user << ", " << errno << std::endl;
            close(conn_socket);
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
			m._user = user;
			// add it to queue
			(this->recv_queue_lock).lock();
			(this->recv_queue).push(std::move(m));
			(this->recv_queue_lock).unlock();
		}
	}
}