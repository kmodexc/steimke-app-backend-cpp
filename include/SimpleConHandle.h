#pragma once
#include "IConHandle.h"
#include <string>
#include <string.h>
#include <unistd.h>

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