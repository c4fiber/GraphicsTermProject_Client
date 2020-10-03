#include "Network.h"

#define TARGET_ADDR "127.0.0.1"
#define PORT 6789

int main() {
	try
	{
		WSASession Session;
		UDPSocket Socket;
		std::string data;
		
		while (1) {
			// stdin
			getline(std::cin, data);
			char buffer[100];

			Socket._sendto(TARGET_ADDR, PORT, data.c_str(), data.size());
			Socket._recvfrom(buffer, 100);
			std::cout << buffer;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
	}
	char c;
	std::cin >> c;

	return 0;
}