#include "Network.hpp"
#include <netinet/in.h>
#include <memory.h>

Network::Addr::Addr()
{
	memset(&(this->addr), 0, sizeof(sockaddr));
}

bool Network::Addr::operator<(const Addr& a)const
{
	return (memcmp(&(this->addr), &(a.addr), sizeof(sockaddr)) < 0);
}