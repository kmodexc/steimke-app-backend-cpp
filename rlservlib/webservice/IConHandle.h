#pragma once
#include <string>

namespace rls{

class IConHandle{
public:
	virtual void send(std::string str) = 0;
};

}