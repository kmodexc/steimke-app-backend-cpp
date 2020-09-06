#include "App.h"
#include "CmakeConfig.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <iostream>
#include <signal.h>

using namespace std;
using namespace rls;

void signal_handler(int signal){
	std::cout << "signal " << signal << " send" << std::endl;
	// flush buffer before end
	if(spdlog::get("rlservlib") != nullptr){
		spdlog::get("rlservlib")->info("signal {} send to end program",signal);
		spdlog::get("rlservlib")->flush();
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	try
	{
		auto logger = spdlog::rotating_logger_mt("rlservlib", "logs/rotating-log.txt",5000000,3);
	}
	catch (const spdlog::spdlog_ex &ex)
	{
		std::cerr << "Log init failed: " << ex.what() << std::endl;
	}

	// report version
	spdlog::get("rlservlib")->info("HeySteimke Server: {} ; Version: {}.{}", argv[0], RLSERV_VERSION_MAJOR, RLSERV_VERSION_MINOR);

	signal(SIGINT,signal_handler);
	signal(SIGTERM,signal_handler);

	App *app = nullptr;
	try
	{
		app = new App();
		if (app->initialize(argc, argv))
			app->run();
		else
		{
			spdlog::get("rlservlib")->error("Application didnt initialize successful");
		}
	}
	catch (exception &exc)
	{
		spdlog::get("rlservlib")->error("Exception in main: {}", exc.what());
	}
	if (app != nullptr)
		delete app;
}