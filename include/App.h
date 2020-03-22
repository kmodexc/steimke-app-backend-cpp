#pragma once
#include "DependencyService.h"
#include "IJSONSerializer.h"
#include <iostream>

namespace rls{

class App : public IConHandler{
	IHTTPSocket* soc;
	IJSONSerializer* ser;
public:
	App();
	bool initialize();
	void run();
	bool get(IConHandle* soc,std::string path) override;
	virtual ~App();
};

}