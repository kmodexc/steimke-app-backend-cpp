#pragma once
#include "IDataBase.h"
#include "DBItem.h"

namespace rls{

class MockDataStore : public IDataBase<Item>{
	std::vector<Item> items;
public:
	MockDataStore();
	Item get(int id) override;
	std::vector<Item> getAll() override;
	~MockDataStore() override;
};


}