#pragma once
#include "DependencyService.h"
#include <iostream>

namespace rls{

class App{
	IHTTPSocket* soc;
public:
	App();
	bool initialize();
	void run();
	~App();
};

}