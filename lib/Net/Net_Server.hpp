#pragma once
#include "Network.hpp"

class Net_Server:public Network
{
protected:
	typedef std::function<bool(const unsigned int, const std::string)> type_func_login;
	void TCP_ACPT_thread(int acpt_socket, type_func_login login);
	void TCP_RECV_thread(int conn_socket, type_func_login login);
public:
	Net_Server(type_func_login login, const unsigned int flag);
};