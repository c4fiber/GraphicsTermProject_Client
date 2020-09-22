#include "Client.hpp"

#include <iostream>

using namespace std;

Client::Client(const char *serverAddr, int port) {
	this->serverAddr = serverAddr;
	this->port = port;
}

void Client::init() {
	WSADATA wsaData;
	int errorStatus;

	errorStatus = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (errorStatus != NO_ERROR)
		throw "WSAStartup error";

	socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket == INVALID_SOCKET) {
		WSACleanup();
		throw "socker error";
	}

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(port);
	tAddr.sin_addr.s_addr = inet_addr(serverAddr);

	if (connect(socket, (SOCKADDR *)&tAddr, sizeof(tAddr)) == SOCKET_ERROR) {
		closesocket(socket);
		WSACleanup();
		throw "connect error";
	}
	cout << "connect" << endl;

	bThreadRun = true;
	recvThread = new thread(&Client::recvLoop, this);
}
void Client::close() {
	if (socket != INVALID_SOCKET)
		closesocket(socket);

	bThreadRun = false;
	if (recvThread != nullptr) {
		if (recvThread->joinable())
			recvThread->join();
		delete recvThread;
		recvThread = nullptr;
	}

	for (auto iter = eventThreadList.begin(); iter != eventThreadList.end(); ) {
		if ((*iter)->joinable()) {
			(*iter)->join();
			iter = eventThreadList.erase(iter);
		}
	}

	WSACleanup();
}

void Client::send(const char *buff, int len) const {
	::send(socket, buff, len, 0);
}
void Client::recvLoop() {
	char buff[BUFF_SIZE] = {};
	int recvLen;

	while (bThreadRun) {
		recvLen = recv(socket, buff, BUFF_SIZE, 0);

		if (recvLen < 0)
			continue;
		else if (recvLen == 0)
			break;

		if (onRecv != nullptr) {
			eventThreadList.push_back(new thread([=]() {
				this->onRecv(buff, recvLen);
			}));
		}

		// Event Thread Á¤¸®
		for (auto iter = eventThreadList.begin(); iter != eventThreadList.end(); ) {
			if (!(*iter)->joinable())
				iter = eventThreadList.erase(iter);
			else
				iter++;
		}
	}

	bThreadRun = false;
}