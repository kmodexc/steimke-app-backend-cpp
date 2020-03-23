#pragma once
#include "Item.h"
#include <vector>

namespace rls{

class IDataBase{
public:
	virtual Item getItem(int id) = 0;
	virtual std::vector<Item> getItems() = 0;
	virtual ~IDataBase(){};
};

}