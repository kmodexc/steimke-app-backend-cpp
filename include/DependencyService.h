#pragma once

#include "IHTTPSocket.h"
#include "HTTPSocket.h"
#include "JSONSerializer.h"
#include "IDataBase.h"
#include "MockDataStore.h"
#include "SQLDataBase.h"

namespace rls{

class DependencyService {
	IHTTPSocket* soc;
	IJSONSerializer* ser;
	IDataBase* db;
public:
	DependencyService();
	~DependencyService();
public:
	IHTTPSocket *getHttpSocket();
	IJSONSerializer *getJSONSerializer();
	IDataBase* getDataBase();
};

}