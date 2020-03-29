#include "DBItem.h"
#include <cstring>

namespace rls
{

int DBItem::global_id_counter = 0;

DBItem::DBItem(std::string name) : Item(++global_id_counter, 
	ItemState::created, name, "", "", -1, -1, 0, 0,-1,TimeStamp(),TimeStamp(),TimeStamp())
{
}

} // namespace rls