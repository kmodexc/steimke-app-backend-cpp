#pragma once

#include "IConHandler.h"

namespace rls
{

class IHTTPSocket
{
public:
	virtual void init(IConHandler *handler, int port) = 0;
	virtual void run() = 0;
	virtual void send(std::string str) = 0;
	virtual void close() = 0;
};

} // namespace rls