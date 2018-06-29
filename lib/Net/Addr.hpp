#pragma once
#define LINUX
#ifdef LINUX
	#include <netinet/in.h>
#endif

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