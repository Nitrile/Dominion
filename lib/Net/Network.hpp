#pragma once

#define UDP 1U
#define TCP 2U
#define TCP_PORT 1999
#define UDP_PORT 0316
#define SERVER_IP "127.0.0.1"

#include "Msg.hpp"
#include "Addr.hpp"
#include <boost/bimap.hpp>
#include <queue>
#include <mutex>
#include <memory>
#include <map>

class Network
{
public:
	void send(Msg m, unsigned int flag);
	Msg recv();
	~Network();
protected:
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
    Msg TCP_recv_helper(int conn_socket);
};