#include "Network.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include "Merge.hpp"
#ifdef WINDOWS
	#define close(sock) closesocket(sock)
	#include <Winsock2.h>
	#include <Ws2tcpip.h>
#endif
#ifdef LINUX
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <arpa/inet.h>
#endif

#define SERVER 0U
#define CLIENT 4U

inline int socket_send(int __fd, const char* __buf, size_t __n, int __flags)
{
	return send(__fd, __buf, __n, __flags);
}
inline int socket_recv(int __fd, char* __buf, size_t __n, int __flags)
{
	return recv(__fd, __buf, __n, __flags);
}

// TCP [+inf]
// size [4] flag [1] pack.#[3] info[1-inf]
// UDP [256]
// size [1] total_pack.#[3] flag [1] pack.#[3] info[1-248]

void Network::send(Msg m, unsigned int flag)
{

	static unsigned int pack_num[3] = {0U, 0U, 0U};

	unsigned int _user;
	unsigned int _arg2;
	int error_code;

	if ((flag & this->flag) == 0U)
	{
		std::cout << "Network:send:flag doesn't match" << std::endl;
		return;
	}

	_user = (this->flag & CLIENT)?0U:m._user;
	_arg2 = (flag << 24) + pack_num[flag];

	pack_num[flag] = (pack_num[flag] + 1) & 0x00FFFFFF;

__TCP:
	if (TCP & flag)
	{
		// find the socket to send.
		(this->TCP_T_lock).lock();
		if ((this->TCP_T).left.count(_user) == 0)
		{
			std::cout << "Network:TCP:send:user doesn't exists, user " << _user << std::endl;
			(this->TCP_T_lock).unlock();
			goto __UDP;
		}
		int send_socket = (this->TCP_T).left.find(_user)->second;
		(this->TCP_T_lock).unlock();

		// send Msg
		unsigned int _size = m._size;
		error_code = socket_send(send_socket, (char *) &_size, 4, 0);
		error_code = socket_send(send_socket, (char *) &_arg2, 4, 0);
		error_code = socket_send(send_socket, m._buf.get(), m._size, 0);
		if (error_code == -1)
		{
			std::cout << "Network:TCP:send:error, user " << _user << ", " << errno << std::endl;
			goto __UDP;
		}
	}
__UDP:
	if (UDP & flag)
	{
		error_code = 0;
		char* _buf;
		// find the Addr to send.
		(this->UDP_T_lock).lock();
		if (!((this->UDP_T).left.count(_user) > 0))
		{
			std::cout << "Network:UDP:send:user doesn't exists, user " << _user << std::endl;
			(this->UDP_T_lock).unlock();
			return;
		}
		Addr send_addr = (this->UDP_T).left.find(_user)->second;
		(this->UDP_T_lock).unlock();

		// divide it to serval package
		unsigned int tp_quo = m._size/248;
		unsigned int tp_rem = m._size%248;
		unsigned int total_pack = (tp_rem)?tp_quo + 1:tp_quo;
		total_pack = total_pack & 0x00FFFFFF;
		unsigned int i;
		for (i = 0U; i < tp_quo; i++)
		{
			_buf = (char*)malloc(256);
			unsigned int _arg1 = (i << 24) + total_pack;
			memcpy(_buf + 0, &_arg1, 4);
			memcpy(_buf + 4, &_arg2, 4);
			memcpy(_buf + 8, m._buf.get() + 248*i, 248);
			error_code = sendto(this->UDP_socket, _buf, 256, 0, &(send_addr.addr), sizeof(sockaddr));
			free(_buf);
		}
		if (tp_rem)
		{
			_buf = (char*)malloc(tp_rem + 8);
			unsigned int _arg1 = (i << 24) + total_pack;
			memcpy(_buf + 0, &_arg1, 4);
			memcpy(_buf + 4, &_arg2, 4);
			memcpy(_buf + 8, m._buf.get() + 248*i, tp_rem);
			error_code = sendto(this->UDP_socket, _buf, tp_rem + 8, 0, &(send_addr.addr), sizeof(sockaddr));
			free(_buf);
		}
		if (error_code == -1)
		{
			std::cout << "Network:UDP:send:error, user " << _user << ", " << errno << std::endl;
			return;
		}
	}
}

Msg Network::recv()
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

Msg Network::TCP_recv_helper(int conn_socket)
{
	int n = 0, t;
	char* buf = (char*)malloc(8);
	while (n < 8)
	{
		t = socket_recv(conn_socket, buf + n, 8 - n, 0);
		if (t <= 0)
		{
			Msg m;
			return m;
		}
		n += t;
	}
	unsigned int _size = *(unsigned int*) buf;
	unsigned int _type = (*(unsigned int*)(buf + 4)) >> 6;
	unsigned int pack_num = (*(unsigned int*)(buf + 4)) & 0x00FFFFFF;
	free(buf);

	n = 0;
	buf = (char*)malloc(_size);
	while (n < _size)
	{
		t = socket_recv(conn_socket, buf + n, _size - n, 0);
		if (t <= 0)
		{
			Msg m;
			return m;
		}
		n += t;
	}

	std::unique_ptr<char[], void(*)(void*)> p(buf, free);
	Msg m(std::move(p), _size, 0);

	return std::move(m);
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