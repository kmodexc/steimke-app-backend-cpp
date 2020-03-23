#pragma once
#include "IDataBase.h"

namespace rls{

class MockDataStore : public IDataBase{
public:
	Item getItem(int id) override;
};


}