#include "JSONSerializer.h"

namespace rls
{

using json = nlohmann::json;

std::string JSONSerializer::toJSON(const Item &item)
{
	json j;
	j["id"] = item.getID();
	j["state"] = "created";// item.getState().toString();
	j["name"] = item.getName();
	j["shortdesc"] = item.getShortDiscription();
	j["desc"] = item.getDescription();
	j["assignedId"] = item.getAssignedID();
	j["creatorId"] = item.getCreatorID();
	j["priority"] = item.getPriority();
	j["workload"] = item.getWorkload();
	return j.dump();
}
std::string JSONSerializer::toJSON(const std::vector<Item> &items)
{
	std::vector<int> idlist;
	for (auto it : items)
	{
		idlist.push_back(it.getID());
	}
	return toJSON(idlist);
}
void JSONSerializer::fromJSON(const std::string &str, Item *itout)
{
	auto j = json::parse(str);
	int id = -1;
	if (!j["id"].is_null())
		id = j["id"].get<int>();
	ItemState state = ItemState::created;
	if (!j["state"].is_null())
		std::string str_state = j["state"].get<std::string>();
	std::string name;
	if (!j["name"].is_null())
		name = j["name"].get<std::string>();
	std::string shortdesc;
	if (!j["shortdesc"].is_null())
		shortdesc = j["shortdesc"].get<std::string>();
	std::string desc;
	if (!j["desc"].is_null())
		desc = j["desc"].get<std::string>();
	int assid = -1;
	if (!j["assignedId"].is_null())
		assid = j["assignedId"].get<int>();
	int creaid = -1;
	if (!j["creatorId"].is_null())
		creaid = j["creatorId"].get<int>();
	int prio = -1;
	if (!j["priority"].is_null())
		prio = j["priority"].get<int>();
	int wl = -1;
	if (!j["workload"].is_null())
		wl = j["workload"].get<int>();

	Item it(id, state, name, shortdesc, desc, assid, creaid, prio, wl);
	*itout = it;
}
std::string JSONSerializer::toJSON(const User &usr)
{
	json j;
	j["id"] = usr.getId();
	j["state"] = "user";// usr.getState().toString();
	j["name"] = usr.getName();
	j["workload"] = usr.getWorkload();
	return j.dump();
}
void JSONSerializer::fromJSON(const std::string &str, User *usr)
{
	auto j = json::parse(str);
	int id = -1;
	if (!j["id"].is_null())
		id = j["id"].get<int>();
	UserState state = UserState::user;
	if (!j["state"].is_null())
		std::string str_state = j["state"].get<std::string>();
	std::string name;
	if (!j["name"].is_null())
		name = j["name"].get<std::string>();
	int wl = -1;
	if (!j["workload"].is_null())
		wl = j["workload"].get<int>();

	User mu(id, name, state, wl);
	*usr = mu;
}
std::string JSONSerializer::toJSON(const std::vector<int> &ids)
{
	json j = ids;
	return j.dump();
}
JSONSerializer::~JSONSerializer()
{
}

} // namespace rls