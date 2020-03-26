#pragma once
#include "IJSONSerializer.h"
#include "nlohmann/json.hpp"

namespace rls
{

class JSONSerializer : public IJSONSerializer
{
public:
	std::string toJSON(const Item &item) override;
	std::string toJSON(const std::vector<Item> &item) override;
	void fromJSON(const std::string &, Item *) override;

	std::string toJSON(const User &item) override;
	void fromJSON(const std::string &, User *it) override;

	std::string toJSON(const std::vector<int> &ids) override;

	~JSONSerializer() override;
};

} // namespace rls