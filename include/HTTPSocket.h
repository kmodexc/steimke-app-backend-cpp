#pragma once
#include "IHTTPSocket.h"

namespace rls{

class HTTPSocket : public IHTTPSocket{
public:
	void init(IConHandler *handler, int port);
	void run();
	void close();
};

}