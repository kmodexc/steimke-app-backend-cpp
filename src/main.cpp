#include "App.h"
#include <iostream>

using namespace std;
using namespace rls;

int main(int argc,char *argv[])
{
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