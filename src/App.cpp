#include "App.h"

using namespace std;

namespace rls
{

App::App()
{
	soc = nullptr;
	ser = nullptr;
}

bool App::initialize()
{
	soc = dep.getHttpSocket();
	ser = dep.getJSONSerializer();
	soc->init(this, 10001);
	cout << "Initialized Socket on port 10001" << endl;
	return true;
}

void App::run()
{
	try
	{
		cout << "Starting Server" << endl;
		soc->run();
		cout << "Server finished without exception" << endl;
	}
	catch (exception exc)
	{
		cerr << "Exception in Application trying to run soccet" << endl;
		cerr << exc.what() << endl;
	}
}

bool App::get(IConHandle *soc, std::string path)
{
	cout << "Incoming GET Request" << endl;
	cout << "Requested Path: " << path << endl;

	// answer if item is requested
	if (path.find("/api/item/") != 0)
	{
		Item it;
		std::string content = ser->toJSON(it);

		std::string message;

		message += "HTTP/1.1 200 OK\r\n";
		message += "Content-Type: text/plain\r\n";
		message += "Content-Length: ";
		message += content.length();
		message += "\r\n";
		message += "\r\n";
		message += content;

		soc->send(message);

		return true;
	}

	// Default answer

	std::string content = "Hello from HelloSteimke RESTles API Server!\nRequested Path: '";
	content += path + "'\n";

	std::string message;

	message += "HTTP/1.1 200 OK\r\n";
	message += "Content-Type: text/plain\r\n";
	message += "Content-Length: ";
	message += content.length();
	message += "\r\n";
	message += "\r\n";
	message += content;

	soc->send(message);

	return true;

	// // create whole message
	// snprintf(message, sizeof(message),
	// 		 "HTTP/1.1 200 OK\r\n"
	// 		 "Content-Type: text/plain\r\n"
	// 		 "Content-Length: %d\r\n" // Always set Content-Length
	// 		 "\r\n"
	// 		 "%s",
	// 		 content_length, content);
}

App::~App()
{
	if (soc != nullptr)
		soc->close();
}

} // namespace rls