#include "JSONSerializer.h"

namespace rls
{

using json = nlohmann::json;

std::string JSONSerializer::toJSON(const Item &item)
{
	json j;
	j["id"] = item.getID();
	j["state"] = toString(item.getState());
	j["name"] = item.getName();
	j["shortdesc"] = item.getShortDiscription();
	j["desc"] = item.getDescription();
	j["assignedId"] = item.getAssignedID();
	j["creatorId"] = item.getCreatorID();
	j["priority"] = item.getPriority();
	j["workload"] = item.getWorkload();
	j["placeid"] = item.getPlaceID();

	addToJSON(j, item.getCreateTime(), "createtime");
	addToJSON(j, item.getAssignedTime(), "assignedtime");
	addToJSON(j, item.getFinishedTime(), "finishedtime");

	return j.dump();
}
void JSONSerializer::fromJSON(const std::string &str, Item *itout)
{
	auto j = json::parse(str);
	int id = -1;
	if (!j["id"].is_null())
		id = j["id"].get<int>();
	ItemState state = ItemState::created;
	if (!j["state"].is_null())
	{
		std::string str_state = j["state"].get<std::string>();
		state = parseItemState(str_state);
	}
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
	int placeid = -1;
	if (!j["placeid"].is_null())
		placeid = j["placeid"].get<int>();

	TimeStamp createTime;
	if (!j["createtime"].is_null())
		getFromJSON(j, &createTime, "createtime");
	TimeStamp assignedTime;
	if (!j["assignedtime"].is_null())
		getFromJSON(j, &createTime, "assignedtime");
	TimeStamp finishedTime;
	if (!j["finishedtime"].is_null())
		getFromJSON(j, &createTime, "finishedtime");

	Item it(id, state, name, shortdesc, desc, assid, creaid, prio, wl, placeid, createTime, assignedTime, finishedTime);
	*itout = it;
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
void JSONSerializer::addToJSON(json &j, const TimeStamp &time, const std::string &prefix)
{
	j[prefix.c_str()]["year"] = time.year;
	j[prefix.c_str()]["month"] = time.month;
	j[prefix.c_str()]["day"] = time.day;
	j[prefix.c_str()]["hour"] = time.hour;
	j[prefix.c_str()]["minute"] = time.minute;
}
void JSONSerializer::getFromJSON(const json &j, TimeStamp *time, const std::string &prefix)
{
	TimeStamp t;
	//memset(&t, 0, sizeof(TimeStamp)); this is done in constructor
	if (!j[prefix.c_str()]["year"].is_null())
		t.year = j[prefix.c_str()]["year"].get<int>();
	if (!j[prefix.c_str()]["month"].is_null())
		t.month = j[prefix.c_str()]["month"].get<int>();
	if (!j[prefix.c_str()]["day"].is_null())
		t.day = j[prefix.c_str()]["day"].get<int>();
	if (!j[prefix.c_str()]["hour"].is_null())
		t.hour = j[prefix.c_str()]["hour"].get<int>();
	if (!j[prefix.c_str()]["minute"].is_null())
		t.minute = j[prefix.c_str()]["minute"].get<int>();
	*time = t;
}

std::string JSONSerializer::toJSON(const User &usr)
{
	json j;
	j["id"] = usr.getId();
	j["state"] = toString(usr.getState());
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
	{
		std::string str_state = j["state"].get<std::string>();
		state = parseUserState(str_state);
	}
	std::string name;
	if (!j["name"].is_null())
		name = j["name"].get<std::string>();
	int wl = -1;
	if (!j["workload"].is_null())
		wl = j["workload"].get<int>();

	User mu(id, name, state, wl);
	*usr = mu;
}
std::string JSONSerializer::toJSON(const Place &p)
{
	json j;
	j["id"] = p.id;
	j["type"] = toString(p.type);
	j["name"] = p.name;
	return j.dump();
}
void JSONSerializer::fromJSON(const std::string &str, Place *p)
{
	auto j = json::parse(str);
	int id = -1;
	if (!j["id"].is_null())
		id = j["id"].get<int>();
	PlaceType type = PlaceType::_public;
	if (!j["type"].is_null())
	{
		std::string str_state = j["type"].get<std::string>();
		type = parsePlaceType(str_state);
	}
	std::string name;
	if (!j["name"].is_null())
		name = j["name"].get<std::string>();

	Place mp;
	mp.id = id;
	mp.type = type;
	mp.name = name;
	*p = mp;
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