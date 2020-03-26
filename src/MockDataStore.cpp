#include "MockDataStore.h"

namespace rls
{

MockDataStore::MockDataStore()
{
	items.push_back(DBItem("a"));
	items.push_back(DBItem("b"));
	items.push_back(DBItem("c"));
	items.push_back(DBItem("d"));
	items.push_back(DBItem("e"));
}

Item MockDataStore::get(int id)
{
	for (Item &i : items)
	{
		if (i.getID() == id)
			return i;
	}
	return Item();
}

std::vector<Item> MockDataStore::getAll()
{
	return items;
}

MockDataStore::~MockDataStore()
{
}

} // namespace rls