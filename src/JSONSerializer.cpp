#include "JSONSerializer.h"

namespace rls{

using json = nlohmann::json;

std::string JSONSerializer::toJSON(const Item& item) {
	json j;
	j["id"] = item.getID();
	j["state"] = (int)item.getState();
	j["name"] = item.getName();
	j["shortdesc"] = item.getShortDiscription();
	j["desc"] = item.getDescription();
	j["assignedId"] = item.getAssignedID();
	j["creatorId"] = item.getCreatorID();
	j["priority"] = item.getPriority();
	j["workload"] = item.getWorkload();
	return j.dump();
}

Item JSONSerializer::fromJSON(const std::string& str) {
	auto j = json::parse(str);
	ItemState state = (ItemState)j["state"].get<int>();
	Item it(j["id"].get<int>(),state,j["name"].get<std::string>(),j["shortdesc"].get<std::string>(),j["desc"].get<std::string>(),j["assignedId"].get<int>(),j["creatorId"].get<int>(),j["priority"].get<int>(),j["workload"].get<int>());
	return it;
}

JSONSerializer::~JSONSerializer(){
	
}

}