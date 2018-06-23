#pragma once

#define LINUX

#include <queue>
#include <boost/bimap.hpp>
#include <mutex>
#include <memory>
#ifdef LINUX
	#include <netinet/in.h>
#endif

#define UDP 1U
#define TCP 2U
#define TCP_PORT 1999
#define UDP_PORT 0316
#define SERVER_IP "127.0.0.1"

class Network;
class Net_Server;
class Net_Client;

class Network
{
public:
	class Msg
	{
	private:
		typedef std::unique_ptr<char[], void(*)(void*)> mptr;
		mptr _buf;
		unsigned int _size;
		unsigned int _user;

	public:
		Msg();
		Msg(mptr buf, unsigned int buf_size, unsigned int user);

		mptr ptr();
		unsigned int size();
		unsigned int user();

		Msg& operator= (const Msg&) = delete;
		Msg& operator= (Msg&& m);
		Msg(const Msg&) = delete;
		Msg(Msg&& m);
	};
	void send(Msg m, int flag);
	Msg recv();
	~Network();
protected:
	class Addr
	{
	public:
		union
		{
			sockaddr addr;
			sockaddr_in addr_in;
			sockaddr_in6 addr_in6;
		};
		Addr();
		bool operator<(const Addr& a) const;
	};

	typedef boost::bimap<unsigned int, int> type_TCP_T;
	typedef boost::bimap<unsigned int, Addr> type_UDP_T;
	type_TCP_T TCP_T;
	type_UDP_T UDP_T;
	int UDP_socket;

	std::queue<Msg> recv_queue;

	std::recursive_mutex recv_queue_lock;
	std::recursive_mutex TCP_T_lock;
	std::recursive_mutex UDP_T_lock;

	unsigned int flag;
};

class Net_Client:public Network
{
protected:
	void TCP_RECV_thread(int conn_socket);
public:
	Net_Client(unsigned int user, std::string passwd, unsigned int flag);
};

class Net_Server:public Network
{
protected:
	typedef std::function<bool(const unsigned int, const std::string)> type_func_login;
	void TCP_ACPT_thread(int acpt_socket, type_func_login login);
	void TCP_RECV_thread(int conn_socket, type_func_login login);
public:
	Net_Server(type_func_login login, const unsigned int flag);
};