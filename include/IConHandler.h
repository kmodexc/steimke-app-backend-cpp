#pragma once

#include <string>
#include "IHTTPSocket.h"

namespace rls
{

class IConHandler
{
public:
	// handler for incoming get request
	virtual bool get(IHTTPSocket* soc,std::string path) = 0;
};

} // namespace rls