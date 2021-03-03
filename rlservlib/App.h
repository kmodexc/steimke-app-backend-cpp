#pragma once
#include "DependencyService.h"
#include "IJSONSerializer.h"
#include <sstream>

namespace rls{

class App : public IConHandler{
	DependencyService dep;
	IHTTPSocket* soc;
	IJSONSerializer* ser;
	IDataBase<Item>* dbitem;
	IDataBase<User>* dbuser;
	IDataBase<Place>* dbplaces;
protected:
	static void ok(IConHandle* con,std::string content);
	static void nok(IConHandle* con,std::string content);
public:
	App();
	bool initialize(int argc,char *argv[]);
	void start();
	void stop();
	bool get(IConHandle* soc,std::string path) override;
	bool put(IConHandle* soc,std::string path,std::string content) override;
	bool post(IConHandle* soc,std::string path,std::string content) override;
	bool del(IConHandle* soc,std::string path) override;
	virtual ~App();

public:
	const int PORT = 443;
};

}