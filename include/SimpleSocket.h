#pragma once
#include "IHTTPSocket.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <thread>
#include <sstream>
#include <IConHandle.h>
#include "SimpleConHandle.h"

namespace rls{

enum class HTTPMethod{
	GET,
	POST,
	PUT,
	DELETE
};

struct HTTPHeader{
	bool valid;
	HTTPMethod method;
	std::string path;
	int contentLength;
};

class SimpleSocket : public IHTTPSocket{
	int sock;
	bool run_thread;
	std::thread* mthread;
	IConHandler* handler;

protected:
	static HTTPHeader decodeHeader(std::string str_header);

public:
	void init(IConHandler *handler, int port) override;
	void run() override;
	void close() override;
	static void main_thread(SimpleSocket* msock);
	~SimpleSocket(){};
	SimpleSocket();
};

}