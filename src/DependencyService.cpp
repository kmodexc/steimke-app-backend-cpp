#include "DependencyService.h"

namespace rls{

IHTTPSocket* DependencyService::getHttpSocket(){
	return new HTTPSocket();
}

}