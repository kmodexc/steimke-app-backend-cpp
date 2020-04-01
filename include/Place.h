#pragma once
#include <string>

namespace rls
{

enum class PlaceType
{
	_public,
	_private
};

std::string toString(const PlaceType& p);
PlaceType parsePlaceType(const std::string& str);

class Place
{
public:
	int id;
	std::string name;
	PlaceType type;
};

} // namespace rls