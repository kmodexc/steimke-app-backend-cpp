#include "App.h"

using namespace std;

namespace rls{

App::App(){
	soc = nullptr;
}

bool App::initialize(){
	soc = DependencyService::getHttpSocket();
	soc->init(nullptr,10001);
	return true;
}

void App::run(){
	try{
		soc->run();
	}catch(exception exc){
		cerr << "Exception in Application trying to run soccet" << endl;
		cerr << exc.what() << endl;
	}
}


App::~App(){
	if(soc != nullptr)
		soc->close();
}

}