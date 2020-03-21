#pragma once

#include "IHTTPSocket.h"

namespace rls{

class DependencyService {
	public:
	static IHTTPSocket *getHttpSocket();
};

}