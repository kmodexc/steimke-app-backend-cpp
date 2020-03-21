#pragma once

#include "IHTTPSocket.h"
#include "HTTPSocket.h"

namespace rls{

class DependencyService {
	public:
	static IHTTPSocket *getHttpSocket();
};

}