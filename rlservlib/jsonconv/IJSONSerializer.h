#pragma once
#include "Item.h"
#include "Place.h"
#include "User.h"
#include <string>
#include <vector>

namespace rls
{

	struct ContainerItemPlaceUser
	{
		std::vector<Item> items;
		std::vector<Place> places;
		std::vector<User> users;
	};

	class IJSONSerializer
	{
	public:
		virtual std::string toJSON(const Item &item) = 0;
		virtual void fromJSON(const std::string &, Item *it) = 0;
		virtual std::string toJSON(const std::vector<Item> &items) = 0;

		virtual std::string toJSON(const Place &place) = 0;
		virtual void fromJSON(const std::string &, Place *it) = 0;

		virtual std::string toJSON(const User &item) = 0;
		virtual void fromJSON(const std::string &, User *it) = 0;

		virtual std::string toJSON(const std::vector<int> &ids) = 0;

		virtual std::string toJSON(const ContainerItemPlaceUser& cont) = 0;

		virtual ~IJSONSerializer(){};
	};

} // namespace rls
