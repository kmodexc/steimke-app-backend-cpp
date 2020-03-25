#include "DependencyService.h"
#include "MockDataStore.h"
#include "SQLDataBase.h"
#include "HTTPSocket.h"
#include "JSONSerializer.h"
#include "SimpleSocket.h"

namespace rls{

DependencyService::DependencyService(){
	soc = nullptr;
	ser = nullptr;
	db = nullptr;
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
		soc = new SimpleSocket();
	return soc;
}

IJSONSerializer* DependencyService::getJSONSerializer(){
	if(ser == nullptr)
		ser = new JSONSerializer();
	return ser;
}

IDataBase* DependencyService::getDataBase(){
	if(db == nullptr)
		db = new SQLDataBase();
	return db;
}

}