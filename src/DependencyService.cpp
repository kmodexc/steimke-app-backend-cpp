#include "DependencyService.h"
#include "MockDataStore.h"
#include "SQLDataBaseItem.h"
#include "HTTPSocket.h"
#include "JSONSerializer.h"
#include "SimpleSocket.h"
#include "SQLDataBaseUser.h"

namespace rls{

DependencyService::DependencyService(){
	soc = nullptr;
	ser = nullptr;
	dbitem = nullptr;
	dbuser = nullptr;
}

DependencyService::~DependencyService(){
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

IHTTPSocket* DependencyService::getHttpSocket(){
	if(soc == nullptr)
		soc = new HTTPSocket();
	return soc;
}

IJSONSerializer* DependencyService::getJSONSerializer(){
	if(ser == nullptr)
		ser = new JSONSerializer();
	return ser;
}

IDataBase<Item>* DependencyService::getDataBaseItem(){
	if(dbitem == nullptr){
		dbitem = new SQLDataBaseItem();
	}
	return dbitem;
}

IDataBase<User>* DependencyService::getDataBaseUser(){
	if(dbuser == nullptr){
		dbuser = new SQLDataBaseUser();
	}
	return dbuser;
}

}