#pragma once
#include <string>
#include "Item.h"
#include "User.h"
#include <vector>

namespace rls{

class IJSONSerializer{
public:
	virtual std::string toJSON(const Item& item) = 0;
	virtual void fromJSON(const std::string&,Item *it) = 0;
	virtual std::string toJSON(const std::vector<Item>& items) = 0;

	virtual std::string toJSON(const User& item) = 0;
	virtual void fromJSON(const std::string&,User *it) = 0;

	virtual std::string toJSON(const std::vector<int>& ids) = 0;

	virtual ~IJSONSerializer(){};
};

}
