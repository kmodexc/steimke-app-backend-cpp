#pragma once
#include "IDataBase.h"
#include "DBItem.h"

namespace rls{

class MockDataStore : public IDataBase{
	std::vector<Item> items;
public:
	MockDataStore();
	Item getItem(int id) override;
	std::vector<Item> getItems() override;
	~MockDataStore() override;
};


}