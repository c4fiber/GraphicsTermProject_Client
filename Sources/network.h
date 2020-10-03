#include <WinSock2.h> //essential socket header
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#pragma once

#define BUFFER_SIZE 100

class WSASession{
public:
	WSASession() {
		int ret = WSAStartup(MAKEWORD(2, 2), &data);
		if (ret != 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup Failed");
	}
	~WSASession() {
		WSACleanup();
	}
private:
	WSAData data;
};

class UDPSocket {
public:
	UDPSocket() {
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock == INVALID_SOCKET)
			throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
	}
	~UDPSocket() {
		closesocket(sock);
	}

	// client to server (address n port)
	void _sendto(const std::string& address, unsigned short port, const char* buffer, int len) {
		sockaddr_in add;
		add.sin_family = AF_INET;
		add.sin_addr.s_addr = inet_addr(address.c_str());
		add.sin_port = htons(port);

		int ret = sendto(sock, buffer, len, 0, reinterpret_cast<SOCKADDR*>(&add), sizeof(add));
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
	}
	// server to client (sockaddr_in object)
	void _sendto(sockaddr_in& address, const char* buffer, int len) {
		int ret = sendto(sock, buffer, len, 0, reinterpret_cast<SOCKADDR*>(&address), sizeof(address));
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
	}
	sockaddr_in _recvfrom(char* buffer, int len) {
		sockaddr_in from;
		int size = sizeof(from);
		int ret = recvfrom(sock, buffer, len, 0, reinterpret_cast<SOCKADDR*>(&from), &size);
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "recvfrom failed");

		// make the buffer zero terminated
		buffer[ret] = 0;
		return from;
	}
	// bind socket-address
	void _bind(unsigned short port) {
		sockaddr_in add;
		add.sin_family = AF_INET;
		add.sin_addr.s_addr = htonl(INADDR_ANY);
		add.sin_port = htons(port);

		int ret = bind(sock, reinterpret_cast<SOCKADDR*>(&add), sizeof(add));
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
	}

private:
	SOCKET sock;
};
