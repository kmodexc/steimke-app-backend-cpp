#include "App.h"

using namespace std;

namespace rls
{

App::App()
{
	soc = nullptr;
	ser = nullptr;
	dbitem = nullptr;
	dbuser = nullptr;
	dbplaces = nullptr;
}

bool App::initialize(int argc,char *argv[])
{
	int port = PORT;
	if (argc == 2){
		port = atoi(argv[1]);
	}
	soc = dep.getHttpSocket();
	ser = dep.getJSONSerializer();
	dbitem = dep.getDataBaseItem();
	dbuser = dep.getDataBaseUser();
	dbplaces = dep.getDataBasePlaces();
	soc->init(this, port);
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
	catch (exception &exc)
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
void App::nok(IConHandle* soc, std::string content){
	std::stringstream ss;
	ss << "HTTP/1.1 404 NoRequestHandler\r\n";
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
	// answer if item is requested
	if (path.find("/api/item/") == 0)
	{
		int itemid = 0;

		sscanf(path.c_str(), "/api/item/%d", &itemid);

		Item it = dbitem->get(itemid);
		std::string content = ser->toJSON(it);
		ok(soc,content);

		return true;
	}

	// answer if items are requested
	if (path.find("/api/items") == 0)
	{
		std::vector<int> it = dbitem->getIDs();
		std::string content = ser->toJSON(it);
		ok(soc,content);
		return true;
	}

	// answer if user is requested
	if (path.find("/api/user/") == 0)
	{
		int userid = 0;

		sscanf(path.c_str(), "/api/user/%d", &userid);

		User it = dbuser->get(userid);
		std::string content = ser->toJSON(it);
		ok(soc,content);

		return true;
	}

	// answer if users are requested
	if (path.find("/api/users") == 0)
	{
		std::vector<int> it = dbuser->getIDs();
		std::string content = ser->toJSON(it);
		ok(soc,content);
		return true;
	}


	// answer if place is requested
	if (path.find("/api/place/") == 0)
	{
		int placeid = 0;

		sscanf(path.c_str(), "/api/place/%d", &placeid);

		Place it = dbplaces->get(placeid);
		std::string content = ser->toJSON(it);
		ok(soc,content);

		return true;
	}

	// answer if place is requested
	if (path.find("/api/places") == 0)
	{
		std::vector<int> it = dbplaces->getIDs();
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
		Item it;
		ser->fromJSON(content,&it);
		dbitem->update(it);
		ok(soc,"");
		return true;
	}else
	if(path.find("/api/user/") == 0){
		User it;
		ser->fromJSON(content,&it);
		dbuser->update(it);
		ok(soc,"");
		return true;
	}else
	if(path.find("/api/place/") == 0){
		Place it;
		ser->fromJSON(content,&it);
		dbplaces->update(it);
		ok(soc,"");
		return true;
	}else{
		nok(soc,"no handler");
		return true;
	}
	return false;
}
bool App::post(IConHandle *soc, std::string path, std::string content)
{
	if(path.find("/api/item") == 0){
		Item it;
		ser->fromJSON(content,&it);

		// check if valid 
		User creator = dbuser->get(it.getCreatorID());
		if(creator.getId() <= 0 || creator.getName().length() <= 3){
			ok(soc,"");
			return false;
		}

		dbitem->add(it);
		ok(soc,"");
		return true;
	}else
	if(path.find("/api/user") == 0){
		User it;
		ser->fromJSON(content,&it);

		// check if valid
		User creator = it;
		if(creator.getId() <= 0 || creator.getName().length() <= 3){
			ok(soc,"");
			return false;
		}

		dbuser->add(it);
		ok(soc,"");
		return true;
	}else
	if(path.find("/api/place") == 0){
		Place it;
		ser->fromJSON(content,&it);

		// check if valid
		User creator = dbuser->get(it.creatorId);
		if(creator.getId() <= 0 || creator.getName().length() <= 3){
			ok(soc,"");
			return false;
		}

		dbplaces->add(it);
		ok(soc,"");
		return true;
	} else{
		nok(soc,"no handler");
		return true;
	}
	return false;
}
bool App::del(IConHandle *soc, std::string path)
{
	if(path.find("/api/item/") == 0){
		int id = -1;
		sscanf(path.c_str(),"/api/item/%d",&id);
		dbitem->del(id);
		ok(soc,"");
		return true;
	}
	if(path.find("/api/user/") == 0){
		int id = -1;
		sscanf(path.c_str(),"/api/user/%d",&id);
		dbuser->del(id);
		ok(soc,"");
		return true;
	}
	if(path.find("/api/place/") == 0){
		int id = -1;
		sscanf(path.c_str(),"/api/place/%d",&id);
		dbplaces->del(id);
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