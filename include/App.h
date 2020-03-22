#pragma once
#include "DependencyService.h"
#include <iostream>

namespace rls{

class App : public IConHandler{
	IHTTPSocket* soc;
public:
	App();
	bool initialize();
	void run();
	bool get(IConHandle* soc,std::string path) override;
	virtual ~App();
};

}