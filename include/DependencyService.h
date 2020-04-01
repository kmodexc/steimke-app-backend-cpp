#pragma once

#include "IHTTPSocket.h"
#include "IDataBase.h"
#include "IJSONSerializer.h"
#include "Place.h"
#include "User.h"
#include "Item.h"

namespace rls{

class DependencyService {
	IHTTPSocket* soc;
	IJSONSerializer* ser;
	IDataBase<Item>* dbitem;
	IDataBase<User>* dbuser;
	IDataBase<Place>* dbplaces;
public:
	DependencyService();
	~DependencyService();
	void dispose();
public:
	IHTTPSocket *getHttpSocket();
	IJSONSerializer *getJSONSerializer();
	IDataBase<Item>* getDataBaseItem();
	IDataBase<User>* getDataBaseUser();
	IDataBase<Place>* getDataBasePlaces();
};

}