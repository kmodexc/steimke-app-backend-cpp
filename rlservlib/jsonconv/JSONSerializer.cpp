#include "JSONSerializer.h"

namespace rls
{

	using json = nlohmann::json;

	json toJSON_int(const Item &item)
	{
		json j;
		j["id"] = item.getID();
		j["state"] = toString(item.getState());
		j["name"] = item.getName();
		j["shortdesc"] = item.getShortDiscription();
		j["desc"] = item.getDescription();
		j["assignedid"] = item.getAssignedID();
		j["creatorid"] = item.getCreatorID();
		j["priority"] = item.getPriority();
		j["workload"] = item.getWorkload();
		j["placeid"] = item.getPlaceID();

		JSONSerializer::addToJSON(j, item.getCreateTime(), "createtime");
		JSONSerializer::addToJSON(j, item.getAssignedTime(), "assignedtime");
		JSONSerializer::addToJSON(j, item.getFinishedTime(), "finishedtime");

		return j;
	}
	std::string JSONSerializer::toJSON(const Item &item)
	{
		return toJSON_int(item).dump();
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
		if (!j["assignedid"].is_null())
			assid = j["assignedid"].get<int>();
		int creaid = -1;
		if (!j["creatorid"].is_null())
			creaid = j["creatorid"].get<int>();
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
			getFromJSON(j, &assignedTime, "assignedtime");
		TimeStamp finishedTime;
		if (!j["finishedtime"].is_null())
			getFromJSON(j, &finishedTime, "finishedtime");

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
	json toJSON_int(const User &usr)
	{
		json j;
		j["id"] = usr.getId();
		j["state"] = toString(usr.getState());
		j["name"] = usr.getName();
		j["pw"] = usr.getPassword();
		j["email"] = usr.getEmail();
		j["workload"] = usr.getWorkload();
		return j;
	}
	std::string JSONSerializer::toJSON(const User &usr)
	{
		return toJSON_int(usr).dump();
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
		std::string pw;
		if (!j["pw"].is_null())
			pw = j["pw"].get<std::string>();
		std::string email;
		if (!j["email"].is_null())
			email = j["email"].get<std::string>();
		int wl = -1;
		if (!j["workload"].is_null())
			wl = j["workload"].get<int>();

		User mu(id, name, pw, email, state, wl);
		*usr = mu;
	}
	json toJSON_int(const Place &p)
	{
		json j;
		j["id"] = p.id;
		j["type"] = toString(p.type);
		j["name"] = p.name;
		j["creatorId"] = p.creatorId;
		j["members"] = p.members;
		return j;
	}
	std::string JSONSerializer::toJSON(const Place &p)
	{
		return toJSON_int(p).dump();
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
		int creatorId = -1;
		if (!j["creatorId"].is_null())
			creatorId = j["creatorId"].get<int>();
		std::vector<int> members;
		if (!j["members"].is_null())
			members = j["members"].get<std::vector<int>>();

		Place mp;
		mp.id = id;
		mp.type = type;
		mp.name = name;
		mp.creatorId = creatorId;
		mp.members = members;
		*p = mp;
	}
	std::string JSONSerializer::toJSON(const std::vector<int> &ids)
	{
		json j = ids;
		return j.dump();
	}
	std::string JSONSerializer::toJSON(const ContainerItemPlaceUser &cont)
	{
		json itemsJson = json::array();
		for (auto myit = cont.items.begin(); myit != cont.items.end(); myit++)
		{
			itemsJson.push_back(toJSON_int(*myit));
		}
		json placesJson = json::array();
		for (auto myit = cont.places.begin(); myit != cont.places.end(); myit++)
		{
			placesJson.push_back(toJSON_int(*myit));
		}
		json usersJson = json::array();
		for (auto myit = cont.users.begin(); myit != cont.users.end(); myit++)
		{
			usersJson.push_back(toJSON_int(*myit));
		}

		json j;
		j["items"] = itemsJson;
		j["places"] = placesJson;
		j["users"] = usersJson;
		return j.dump();
	}
	JSONSerializer::~JSONSerializer()
	{
	}

} // namespace rls