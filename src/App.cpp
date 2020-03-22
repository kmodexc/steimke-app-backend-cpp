#include "App.h"

using namespace std;

namespace rls
{

App::App()
{
	soc = nullptr;
}

bool App::initialize()
{
	soc = DependencyService::getHttpSocket();
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
	char content[100];
	char message[200];

	// Prepare the message we're going to send
	int content_length = snprintf(content, sizeof(content),
								  "Hello from HelloSteimke RESTles API Server!");

	// create whole message
	snprintf(message, sizeof(message),
			 "HTTP/1.1 200 OK\r\n"
			 "Content-Type: text/plain\r\n"
			 "Content-Length: %d\r\n" // Always set Content-Length
			 "\r\n"
			 "%s",
			 content_length, content);

	soc->send(message);

	cout << "Incoming GET Request" << endl;

	return true;
}

App::~App()
{
	if (soc != nullptr)
		soc->close();
}

} // namespace rls