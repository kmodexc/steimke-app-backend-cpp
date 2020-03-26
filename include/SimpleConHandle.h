#pragma once
#include "IConHandle.h"
#include <string>
#include <string.h>
#include <sys/socket.h>

namespace rls{

class SimpleConHandle : public IConHandle{
	int sockfd;
public:
	SimpleConHandle(int socfd);
	void send(std::string str);
	std::string recv();
	std::string recv(int maxlen);
};

}