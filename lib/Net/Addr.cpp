#include "Addr.hpp"
#include <memory.h>
#ifdef WINDOWS
	#include <Winsock2.h>
	#include <Ws2tcpip.h>
#endif
#ifdef LINUX
	#include <netinet/in.h>
#endif


Addr::Addr()
{
	memset(&(this->addr), 0, sizeof(sockaddr));
}

bool Addr::operator<(const Addr& a)const
{
	return (memcmp(&(this->addr), &(a.addr), sizeof(sockaddr)) < 0);
}