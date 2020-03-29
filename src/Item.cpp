#include "Item.h"
#include <cstring>

namespace rls
{

std::string toString(const ItemState &state)
{
	switch (state)
	{
	case ItemState::aborted:
		break;
		return "aborted";
	case ItemState::assigned:
		return "assigned";
		break;
	default:
	case ItemState::created:
		return "created";
		break;
	case ItemState::finished:
		return "finished";
		break;
	case ItemState::inprogress:
		return "inprogress";
		break;
	case ItemState::paused:
		return "paused";
		break;
	}
	return "created";
}
ItemState parseItemState(const std::string &str)
{
	if (str == "aborted")
	{
		return ItemState::aborted;
	}
	else if (str == "assigned")
	{
		return ItemState::assigned;
	}
	else if (str == "created")
	{
		return ItemState::created;
	}
	else if (str == "finished")
	{
		return ItemState::finished;
	}
	else if (str == "inprogress")
	{
		return ItemState::inprogress;
	}
	else if (str == "paused")
	{
		return ItemState::paused;
	}
	return ItemState::created;
}

Item::Item()
{
	this->id = -1;
	this->state = ItemState::created;
	this->assid = -1;
	this->creaid = -1;
	this->prio = 0;
	this->wl = 0;
	this->placeid = -1;

	memset(&(this->createTime),0,sizeof(TimeStamp));
	memset(&(this->assignedTime),0,sizeof(TimeStamp));
	memset(&(this->finishedTime),0,sizeof(TimeStamp));
}
Item::Item(int id, ItemState state, std::string name,
		   std::string shortdesc, std::string desc,
		   int assid, int creaid, int prio, int wl, int placeid,
		   TimeStamp created, TimeStamp assigned, TimeStamp finished)
{
	this->id = id;
	this->state = state;
	this->name = name;
	this->shortdesc = shortdesc;
	this->desc = desc;
	this->assid = assid;
	this->creaid = creaid;
	this->prio = prio;
	this->wl = wl;
	this->placeid = placeid;

	this->createTime = created;
	this->assignedTime = assigned;
	this->finishedTime = finished;
}
int Item::getID() const
{
	return id;
}
ItemState Item::getState() const
{
	return state;
}
std::string Item::getName() const
{
	return name;
}
std::string Item::getShortDiscription() const
{
	return shortdesc;
}
std::string Item::getDescription() const
{
	return desc;
}
int Item::getAssignedID() const
{
	return assid;
}
int Item::getCreatorID() const
{
	return creaid;
}
int Item::getPriority() const
{
	return prio;
}
int Item::getWorkload() const
{
	return wl;
}
int Item::getPlaceID() const
{
	return placeid;
}
TimeStamp Item::getCreateTime() const
{
	return createTime;
}
TimeStamp Item::getAssignedTime() const
{
	return assignedTime;
}
TimeStamp Item::getFinishedTime() const
{
	return finishedTime;
}

} // namespace rls