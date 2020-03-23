#pragma once
#include <string>
#include "Item.h"
#include <vector>

namespace rls{

class IJSONSerializer{
public:
	virtual std::string toJSON(const Item& item) = 0;
	virtual Item fromJSON(const std::string&) = 0;
	virtual std::string toJSON(const std::vector<Item>& items) = 0;
	virtual ~IJSONSerializer(){};
};

}
