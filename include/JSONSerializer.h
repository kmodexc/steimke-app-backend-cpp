#pragma once
#include "IJSONSerializer.h"
#include "nlohmann/json.hpp"

namespace rls{

class JSONSerializer : public IJSONSerializer{
public:
	std::string toJSON(const Item& item) override;
	std::string toJSON(const std::vector<Item>& item) override;
	Item fromJSON(const std::string&) override;
	~JSONSerializer() override;
};


}