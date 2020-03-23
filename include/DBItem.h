#pragma once
#include "Item.h"

namespace rls{

class DBItem : public Item{
	static int global_id_counter;
public:
	DBItem(std::string name);
};


}