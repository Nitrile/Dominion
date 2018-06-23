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

#define UDP 1U
#define TCP 2U
#define TCP_PORT 1999
#define UDP_PORT 0316
#define SERVER_IP "127.0.0.1"

#define SERVER 0U
#define CLIENT 4U

ssize_t socket_send(int __fd, const void* __buf, size_t __n, int __flags)
{
	return send(__fd, __buf, __n, __flags);
}
ssize_t socket_recv(int __fd, void* __buf, size_t __n, int __flags)
{
	return recv(__fd, __buf, __n, __flags);
}

void Network::send(Msg m, int flag)
{
	int user;
	if ((flag & this->flag) == 0)
	{
		std::cout << "Network:send:flag doesn't match" << std::endl;
		return;
	}

	if (CLIENT & this->flag)
	{
		user = 0;
	}
	else
	{
		user = m.user();
	}

	int error_code;
	if (TCP & flag)
	{

		(this->TCP_T_lock).lock();
		if (!((this->TCP_T).left.count(m.user()) > 0))
		{
			std::cout << "Network:TCP:send:user doesn't exists, user " << m.user() << std::endl;
			(this->TCP_T_lock).unlock();
			return;
		}
		int send_socket = (this->TCP_T).left.find(m.user())->second;
		(this->TCP_T_lock).unlock();

		error_code = socket_send(send_socket, m.ptr().get(), m.size(), 0);
		if (error_code == -1)
		{
			std::cout << "Network:TCP:send:error, user " << m.user() << ", " << errno << std::endl;
		}
	}
	if (UDP & flag)
	{

	}
}

Network::Msg Network::recv()
{
	(this->recv_queue_lock).lock();
	if ((this->recv_queue).empty())
	{
		(this->recv_queue_lock).unlock();
		std::cout << "Network:recv:no Msg" << std::endl;
		Msg m;
		return std::move(m);
	}
	else
	{
		Msg m(std::move(this->recv_queue.front()));
		(this->recv_queue).pop();
		(this->recv_queue_lock).unlock();
		return std::move(m);
	}
}

Network::~Network()
{
	if (TCP & flag)
	{
		TCP_T_lock.lock();
		// TCP
		type_TCP_T::const_iterator iter, iend;
		// close all TCP connection
		for (iter = (this->TCP_T).begin(), iend = (this->TCP_T).end();
		     iter != iend; ++iter)
		{
			std::cout << "Network:TCP:destructor:close " << iter->right << std::endl;
			close(iter->right);
		}

		TCP_T_lock.unlock();
	}
	if (UDP & flag)
	{
		// UDP
	}
}