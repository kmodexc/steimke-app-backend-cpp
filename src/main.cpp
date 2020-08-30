#include "App.h"
#include "CmakeConfig.h"
#include <iostream>

using namespace std;
using namespace rls;

int main(int argc,char *argv[])
{
	// report version
    std::cout << argv[0] << " Version " << RLSERV_VERSION_MAJOR << "."
              << RLSERV_VERSION_MINOR << std::endl;

	App *app = nullptr;
	try{
		app = new App();
		if(app->initialize(argc,argv))
			app->run();
		else
		{
			cerr << "Application didnt initialize successful" << endl;
		}
		
	}catch(exception &exc){
		cerr << "Exception in main" << endl;
		cerr << exc.what() << endl;
	}
	if(app != nullptr)
		delete app;
}