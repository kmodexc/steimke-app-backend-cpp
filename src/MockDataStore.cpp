#include "MockDataStore.h"

namespace rls
{

Item MockDataStore::getItem(int id)
{
	return Item();
}

MockDataStore::~MockDataStore(){
	
}

} // namespace rls