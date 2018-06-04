#include <iostream>

#include <Net/net.hpp>

int main(int argc, char* argv[])
{
	std::cout << "Server!\n";
	std::cout << "Networking library version: " << Net::version() << '\n';
	return EXIT_SUCCESS;
}
