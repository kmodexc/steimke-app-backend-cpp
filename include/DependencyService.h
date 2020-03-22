#pragma once

#include "IHTTPSocket.h"
#include "HTTPSocket.h"
#include "JSONSerializer.h"

namespace rls{

class DependencyService {
	IHTTPSocket* soc;
	IJSONSerializer* ser;
public:
	DependencyService();
	~DependencyService();
public:
	IHTTPSocket *getHttpSocket();
	IJSONSerializer *getJSONSerializer();
};

}