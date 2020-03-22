#pragma once
#include "IJSONSerializer.h"
#include "nlohmann/json.hpp"

namespace rls{

class JSONSerializer : public IJSONSerializer{
public:
	std::string toJSON(const Item& item) override;
	Item fromJSON(const std::string&) override;
};


}