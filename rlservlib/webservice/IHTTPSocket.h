#pragma once

#include "IConHandler.h"

namespace rls
{

class IHTTPSocket
{
public:
	virtual void init(IConHandler *handler, int port) = 0;
	virtual void run() = 0;
	virtual void close() = 0;
	virtual ~IHTTPSocket(){};
};

} // namespace rls