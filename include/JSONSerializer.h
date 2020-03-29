#pragma once
#include "IJSONSerializer.h"
#include "nlohmann/json.hpp"

namespace rls
{

class JSONSerializer : public IJSONSerializer
{
public:
	std::string toJSON(const Item &item) override;
	std::string toJSON(const User &item) override;
	std::string toJSON(const nlohmann::json& j,const TimeStamp &time,const std::string &prefix);
	std::string toJSON(const std::vector<int> &ids) override;
	std::string toJSON(const std::vector<Item> &item) override;

	void fromJSON(const std::string &, Item *) override;
	void fromJSON(const std::string &, User *it) override;
	void fromJSON(const nlohmann::json& j,const std::string &, TimeStamp *it,const std::string &prefix);

	~JSONSerializer() override;
};

} // namespace rls