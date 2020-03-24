#include "App.h"

using namespace std;

namespace rls
{

App::App()
{
	soc = nullptr;
	ser = nullptr;
	db = nullptr;
}

bool App::initialize()
{
	soc = dep.getHttpSocket();
	ser = dep.getJSONSerializer();
	db = dep.getDataBase();
	soc->init(this, PORT);
	cout << "Initialized Socket on port " << PORT << endl;
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
void App::ok(IConHandle* soc, std::string content)
{
	std::stringstream ss;
	ss << "HTTP/1.1 200 OK\r\n";
	ss << "Content-Type: application/json\r\n";
	ss << "Content-Length: " << content.length() << "\r\n";
	ss << "\r\n";
	ss << content;

	std::string message = ss.str();

	cout << "Sending this:" << endl;
	cout << message << endl;

	soc->send(message); 
}
bool App::get(IConHandle *soc, std::string path)
{
	cout << "Incoming GET Request" << endl;
	cout << "Requested Path: " << path << endl;

	// answer if item is requested
	if (path.find("/api/item/") == 0)
	{
		int itemid = 0;

		sscanf(path.c_str(), "/api/item/%d", &itemid);

		Item it = db->getItem(itemid);
		std::string content = ser->toJSON(it);
		ok(soc,content);

		return true;
	}

	// answer if item is requested
	if (path.find("/api/items") == 0)
	{
		std::vector<Item> it = db->getItems();
		std::string content = ser->toJSON(it);
		ok(soc,content);
		return true;
	}

	// Default answer

	std::string content = "Hello from HelloSteimke RESTles API Server!\nRequested Path: '";
	content += path + "'\n";
	ok(soc,content);

	return true;
}
bool App::put(IConHandle *soc, std::string path, std::string content)
{
	if(path.find("/api/item/") == 0){
		Item it = ser->fromJSON(content);
		db->updateItem(it);
		ok(soc,"");
		return true;
	}
	return false;
}
bool App::post(IConHandle *soc, std::string path, std::string content)
{
	if(path.find("/api/item") == 0){
		Item it = ser->fromJSON(content);
		db->addItem(it);
		ok(soc,"");
		return true;
	}
	return false;
}
bool App::del(IConHandle *soc, std::string path)
{
	if(path.find("/api/item/") == 0){
		int id = -1;
		sscanf(path.c_str(),"/item/%d",&id);
		db->deleteItem(id);
		ok(soc,"");
		return true;
	}
	return false;
}
App::~App()
{
	if (soc != nullptr)
		soc->close();
}

} // namespace rls