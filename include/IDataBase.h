#pragma once
#include "Item.h"
#include <vector>

namespace rls{

class IDataBase{
public:
	virtual Item getItem(int id) = 0;
	virtual std::vector<Item> getItems() = 0;
	virtual std::vector<int> getIDs() = 0;
	virtual void addItem(Item) = 0;
	virtual void updateItem(Item) = 0;
	virtual void deleteItem(int id) = 0;
	virtual ~IDataBase(){};
};

}