#pragma once
#include "IHTTPSocket.h"

namespace rls{

class IConHandle{
public:
	IHTTPSocket* soc;
	virtual void send(std::string str) = 0;
};

}