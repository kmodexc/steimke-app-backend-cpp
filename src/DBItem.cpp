#include "DBItem.h"

namespace rls
{

int DBItem::global_id_counter = 0;

DBItem::DBItem(std::string name) : Item(++global_id_counter, ItemState::notassigned, name, "", "", -1, -1, 0, 0)
{
}

} // namespace rls