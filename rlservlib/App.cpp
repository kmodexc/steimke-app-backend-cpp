#include "App.h"
#include "spdlog/spdlog.h"

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

	bool App::initialize(int argc, char *argv[])
	{
		int port = PORT;
		if (argc == 2)
		{
			port = atoi(argv[1]);
		}
		soc = dep.getHttpSocket();
		ser = dep.getJSONSerializer();
		dbitem = dep.getDataBaseItem();
		dbuser = dep.getDataBaseUser();
		dbplaces = dep.getDataBasePlaces();
		soc->init(this, port);
		return (soc != nullptr) 
		&& (ser != nullptr) 
		&& (dbitem != nullptr) 
		&& (dbuser != nullptr) 
		&& (dbplaces!= nullptr);
	}

	void App::start()
	{
		try
		{
			spdlog::get("rlservlib")->info("Starting Server");
			soc->start();
			spdlog::get("rlservlib")->info("Server started without exception");
		}
		catch (std::exception &exc)
		{
			spdlog::get("rlservlib")->error("Exception in Application trying to run soccet. Message={}", exc.what());
		}
	}
	void App::stop()
	{
		try
		{
			spdlog::get("rlservlib")->info("Stopping Server");
			soc->stop();
			spdlog::get("rlservlib")->info("Server stopped without exception");
		}
		catch (std::exception &exc)
		{
			spdlog::get("rlservlib")->error("Exception in Application trying to run soccet. Message={}", exc.what());
		}
	}
	void App::ok(IConHandle *soc, std::string content)
	{
		if (soc == nullptr)
			return;

		std::stringstream ss;
		ss << "HTTP/1.1 200 OK\r\n";
		ss << "Content-Type: application/json\r\n";
		ss << "Content-Length: " << content.length() << "\r\n";
		ss << "\r\n";
		ss << content;

		std::string message = ss.str();

		spdlog::get("rlservlib")->debug("Sending this: {}", message);

		soc->send(message);
	}
	void App::nok(IConHandle *soc, std::string content)
	{
		if (soc == nullptr)
			return;

		std::stringstream ss;
		ss << "HTTP/1.1 404 SomeError\r\n";
		ss << "Content-Type: application/json\r\n";
		ss << "Content-Length: " << content.length() << "\r\n";
		ss << "\r\n";
		ss << content;

		std::string message = ss.str();

		spdlog::get("rlservlib")->debug("Sending this: {}", message);

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
			ok(soc, content);

			return true;
		}

		// answer if items are requested
		if (path.find("/api/items") == 0)
		{
			std::vector<int> it = dbitem->getIDs();
			std::string content = ser->toJSON(it);
			ok(soc, content);
			return true;
		}

		// answer if user is requested
		if (path.find("/api/user/") == 0)
		{
			int userid = 0;

			sscanf(path.c_str(), "/api/user/%d", &userid);

			User it = dbuser->get(userid);
			std::string content = ser->toJSON(it);
			ok(soc, content);

			return true;
		}

		// answer if users are requested
		if (path.find("/api/users") == 0)
		{
			std::vector<int> it = dbuser->getIDs();
			std::string content = ser->toJSON(it);
			ok(soc, content);
			return true;
		}

		// answer if place is requested
		if (path.find("/api/place/") == 0)
		{
			int placeid = 0;

			sscanf(path.c_str(), "/api/place/%d", &placeid);

			Place it = dbplaces->get(placeid);
			std::string content = ser->toJSON(it);
			ok(soc, content);

			return true;
		}

		// answer if place is requested
		if (path.find("/api/places") == 0)
		{
			std::vector<int> it = dbplaces->getIDs();
			std::string content = ser->toJSON(it);
			ok(soc, content);
			return true;
		}

		// Default answer

		std::string content = "Hello from HelloSteimke RESTles API Server!\nRequested Path: '";
		content += path + "'\n";
		ok(soc, content);

		return true;
	}
	bool App::put(IConHandle *soc, std::string path, std::string content)
	{
		if (path.find("/api/item/") == 0)
		{
			Item it;
			ser->fromJSON(content, &it);
			dbitem->update(it);
			ok(soc, "put item ok");
			return true;
		}
		else if (path.find("/api/user/") == 0)
		{
			User it;
			ser->fromJSON(content, &it);
			dbuser->update(it);
			ok(soc, "put user ok");
			return true;
		}
		else if (path.find("/api/place/") == 0)
		{
			Place it;
			ser->fromJSON(content, &it);
			dbplaces->update(it);
			ok(soc, "put place ok");
			return true;
		}
		else
		{
			nok(soc, std::string("no handler (path:") + path + std::string(" ; type PUT)"));
			return true;
		}
		return false;
	}
	bool App::post(IConHandle *soc, std::string path, std::string content)
	{
		if (path.find("/api/item") == 0)
		{
			Item it;
			ser->fromJSON(content, &it);

			// check creator user exists (otherwise this might be a hacker)
			User creator = dbuser->get(it.getCreatorID());
			if (creator.getId() <= 0)
			{
				nok(soc, "not authorized");
				return false;
			}
			// check if valid item (id and name)
			if (it.getID() <= 0 || it.getName().length() <= 3)
			{
				nok(soc, "item not valid");
				return false;
			}
			// check if item already exists
			if (dbitem->get(it.getID()).getID() > 0)
			{
				nok(soc, "item already exists");
				return false;
			}

			dbitem->add(it);
			ok(soc, "post item ok");
			return true;
		}
		else if (path.find("/api/user") == 0)
		{
			User it;
			ser->fromJSON(content, &it);

			// check if valid user (id and name)
			if (it.getId() <= 0 || it.getName().length() <= 3)
			{
				nok(soc, "user not valid");
				return false;
			}
			// check if user already exists
			if (dbuser->get(it.getId()).getId() > 0)
			{
				nok(soc, "user already exists");
				return false;
			}

			dbuser->add(it);
			ok(soc, "post user ok");
			return true;
		}
		else if (path.find("/api/place") == 0)
		{
			Place it;
			ser->fromJSON(content, &it);

			// check creator user exists (otherwise this might be a hacker)
			User creator = dbuser->get(it.creatorId);
			if (creator.getId() <= 0)
			{
				nok(soc, "not authorized");
				return false;
			}
			// check if valid place (id and name)
			if (it.id <= 0 || it.name.length() <= 3)
			{
				nok(soc, "place not valid");
				return false;
			}
			// check if place already exists
			if (dbplaces->get(it.id).id > 0)
			{
				nok(soc, "place already exists");
				return false;
			}

			dbplaces->add(it);
			ok(soc, "post place ok");
			return true;
		}
		else
		{
			nok(soc, std::string("no handler (path:") + path + std::string(" ; type POST)"));
			return true;
		}
		return false;
	}
	bool App::del(IConHandle *soc, std::string path)
	{
		if (path.find("/api/item/") == 0)
		{
			int id = -1;
			sscanf(path.c_str(), "/api/item/%d", &id);
			dbitem->del(id);
			ok(soc, "del item ok");
			return true;
		}
		if (path.find("/api/user/") == 0)
		{
			int id = -1;
			sscanf(path.c_str(), "/api/user/%d", &id);
			dbuser->del(id);
			ok(soc, "del user ok");
			return true;
		}
		if (path.find("/api/place/") == 0)
		{
			int id = -1;
			sscanf(path.c_str(), "/api/place/%d", &id);
			dbplaces->del(id);
			ok(soc, "del place ok");
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