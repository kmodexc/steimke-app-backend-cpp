#pragma once
#include "Item.h"

namespace rls{

class IDataBase{
public:
	virtual Item getItem(int id) = 0;
};

}