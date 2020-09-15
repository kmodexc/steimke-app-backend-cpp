#include "DependencyService.h"
#include "JSONSerializer.h"
#include "MockDataStore.h"
#include "SQLDataBaseItem.h"
#include "SQLDataBaseUser.h"
#include "SQLDataBasePlaces.h"
#include "SimpleSocket.h"
#include "SocketYhirose.h"

namespace rls
{

DependencyService::DependencyService()
{
	soc = nullptr;
	ser = nullptr;
	dbitem = nullptr;
	dbuser = nullptr;
	dbplaces = nullptr;
}

DependencyService::~DependencyService()
{
	dispose();
}

void DependencyService::dispose()
{
	// ***************todo*********************
	/*
	if(soc != nullptr)
		delete soc;
	if(ser != nullptr)
		delete ser;
	if(db != nullptr)
		delete ser;
	*/
}

IHTTPSocket *DependencyService::getHttpSocket()
{
	if (soc == nullptr)
		soc = new SocketYhirose();
	return soc;
}

IJSONSerializer *DependencyService::getJSONSerializer()
{
	if (ser == nullptr)
		ser = new JSONSerializer();
	return ser;
}

IDataBase<Item> *DependencyService::getDataBaseItem()
{
	if (dbitem == nullptr)
	{
		dbitem = new SQLDataBaseItem();
	}
	return dbitem;
}

IDataBase<User> *DependencyService::getDataBaseUser()
{
	if (dbuser == nullptr)
	{
		dbuser = new SQLDataBaseUser();
	}
	return dbuser;
}

IDataBase<Place> *DependencyService::getDataBasePlaces()
{
	if (dbplaces == nullptr)
	{
		dbplaces = new SQLDataBasePlaces();
	}
	return dbplaces;
}

} // namespace rls