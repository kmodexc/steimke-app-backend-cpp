#include "App.h"
#include "CmakeConfig.h"
#include "spdlog/spdlog.h"

using namespace std;
using namespace rls;

int main(int argc,char *argv[])
{
	// report version
    spdlog::info("HeySteimke Server: {} ; Version: {}.{}", argv[0],RLSERV_VERSION_MAJOR,RLSERV_VERSION_MINOR);

	App *app = nullptr;
	try{
		app = new App();
		if(app->initialize(argc,argv))
			app->run();
		else
		{
			spdlog::error("Application didnt initialize successful");
		}
		
	}catch(exception &exc){
		spdlog::error("Exception in main: {}",exc.what());
	}
	if(app != nullptr)
		delete app;
}