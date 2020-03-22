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
	if(_GlobDepServ.soc == nullptr)
		_GlobDepServ.soc = new HTTPSocket();
	return _GlobDepServ.soc;
}

IJSONSerializer* DependencyService::getJSONSerializer(){
	if(_GlobDepServ.ser == nullptr)
		_GlobDepServ.ser = new JSONSerializer();
	return _GlobDepServ.ser;
}

}