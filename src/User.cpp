#include "User.h"

namespace rls
{
User::User() : User(0,"",UserState::guest,0)
{
}
User::User(int id, std::string name, UserState state, int workload)
{
	this->id = id;
	this->name = name;
	this->state = state;
	this->workload = workload;
}
int User::getId() const
{
	return id;
}
std::string User::getName() const
{
	return name;
}
UserState User::getState() const
{
	return state;
}
int User::getWorkload() const
{
	return workload;
}

} // namespace rls
