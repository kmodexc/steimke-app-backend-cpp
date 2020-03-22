#pragma once
#include <string>
#include "Item.h"

namespace rls{

class IJSONSerializer{
public:
	virtual std::string toJSON(const Item& item) = 0;
	virtual Item fromJSON(const std::string&) = 0;
};

}
