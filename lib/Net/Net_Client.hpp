#pragma once
#include "Network.hpp"
#include <string>

class Net_Client:public Network
{
protected:
	void TCP_RECV_thread(int conn_socket);
    void UDP_RECV_thread();
public:
	Net_Client(unsigned int user, std::string passwd, unsigned int flag);
};