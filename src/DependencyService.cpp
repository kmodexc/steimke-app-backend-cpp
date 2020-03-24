#include "DependencyService.h"

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
		soc = new HTTPSocket();
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