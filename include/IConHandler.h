#pragma once

#include <string>

namespace rls
{

class IConHandler
{
	// handler for incoming get request
	virtual bool get(std::string path) = 0;
};

} // namespace rls