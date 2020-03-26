#pragma once

#include "IHTTPSocket.h"
#include "IDataBase.h"
#include "IJSONSerializer.h"

namespace rls{

class DependencyService {
	IHTTPSocket* soc;
	IJSONSerializer* ser;
	IDataBase<Item>* dbitem;
	IDataBase<User>* dbuser;
public:
	DependencyService();
	~DependencyService();
public:
	IHTTPSocket *getHttpSocket();
	IJSONSerializer *getJSONSerializer();
	IDataBase<Item>* getDataBaseItem();
	IDataBase<User>* getDataBaseUser();
};

}