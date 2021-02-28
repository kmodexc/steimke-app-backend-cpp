#include "App.h"
#include "CmakeConfig.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <signal.h>
#include <thread>
#include <stdexcept>
#include <unistd.h>

using namespace std;
using namespace rls;

App *glob_app = nullptr;

void signal_handler(int signal)
{
	std::cout << "signal " << signal << " send" << std::endl;
	// flush buffer before end
	if (spdlog::get("rlservlib") != nullptr)
	{
		spdlog::get("rlservlib")->info("signal {} send to end program", signal);
		spdlog::get("rlservlib")->flush();
	}
	if (glob_app != nullptr)
	{
		glob_app->stop();
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	glob_app = nullptr;

	try
	{
		auto logger = spdlog::stdout_color_mt("rlservlib");
		//auto logger = spdlog::rotating_logger_mt("rlservlib", "data/logs/rotating-log.txt", 5000000, 3);
		logger->set_level(spdlog::level::info);
	}
	catch (const spdlog::spdlog_ex &ex)
	{
		std::cerr << "Log init failed: " << ex.what() << std::endl;
	}

	// report version
	spdlog::get("rlservlib")->info("HeySteimke Server: {} ; Version: {}.{}", argv[0], RLSERV_VERSION_MAJOR, RLSERV_VERSION_MINOR);

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	App *app = nullptr;
	try
	{
		app = new App();
		if (app->initialize(argc, argv))
		{
			app->start();

			glob_app = app;

			std::string line;
			do {
				sleep(1);
				std::getline(std::cin, line);
			} while (line.find("quit rlserv") != 0);

			glob_app = nullptr;

			app->stop();
		}
		else
		{
			spdlog::get("rlservlib")->error("Application didnt initialize successful");
		}
	}
	catch (const std::exception &exc)
	{
		spdlog::get("rlservlib")->error("Exception in main: {}", exc.what());
		spdlog::get("rlservlib")->flush();
		return EXIT_FAILURE;
	}
	if (app != nullptr)
		delete app;
	spdlog::get("rlservlib")->error("Exiting normally");
	spdlog::get("rlservlib")->flush();
	return EXIT_SUCCESS;
}