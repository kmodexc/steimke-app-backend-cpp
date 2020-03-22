#include "Item.h"

namespace rls
{

Item::Item(){
	this->id = -1;
	this->state = ItemState::notassigned;
	this->assid = -1;
	this->creaid = -1;
	this->prio = 0;
	this->wl = 0;
}
Item::Item(int id,ItemState state,std::string name,std::string shortdesc,std::string desc,int assid,int creaid,int prio,int wl)
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

} // namespace rls