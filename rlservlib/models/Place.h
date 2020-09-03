#pragma once
#include <string>
#include <vector>

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
	int creatorId;
	std::vector<int> members;
};

} // namespace rls