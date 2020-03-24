#pragma once

#include "IHTTPSocket.h"
#include "IDataBase.h"
#include "IJSONSerializer.h"

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