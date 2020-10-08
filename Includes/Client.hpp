#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <thread>
#include <list>

#pragma comment(lib, "ws2_32")

using namespace std;

constexpr auto BUFF_SIZE = 1024;

class Client {
public:
	Client(const char *serverAddr, int port);

	void init();
	void close();

	void send(const char *buff, int len) const;
	void recvLoop();

	// `edit`
	void sendU(const char *buff, int len) const;
	void recvLoopU();

	inline void setOnRecv(void (*onRecv)(const char *, const int &)) {
		this->onRecv = onRecv;
	}

	inline bool checkConnect() {
		return bThreadRun;
	}

private:
	SOCKET socket = INVALID_SOCKET;
	
	// `edit`
	SOCKET socketU = INVALID_SOCKET; 

	const char *serverAddr;
	int port;

	bool bThreadRun = false;
	thread *recvThread = nullptr;
	list<thread *> eventThreadList;

	void (*onRecv)(const char *, const int &) = nullptr;
};