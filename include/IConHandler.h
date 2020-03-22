#pragma once

#include <string>
#include "IConHandle.h"

namespace rls
{

class IConHandler
{
public:
	// handler for incoming get request
	virtual bool get(IConHandle* soc,std::string path) = 0;
};

} // namespace rls