#include "DependencyService.h"

namespace rls{

DependencyService::DependencyService(){
	soc = nullptr;
	ser = nullptr;
}

DependencyService::~DependencyService(){
	if(soc != nullptr)
		delete soc;
	if(ser != nullptr)
		delete ser;
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

}