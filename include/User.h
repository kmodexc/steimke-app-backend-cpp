#pragma once
#include <string>

namespace rls
{

enum class UserState
{
	admin,
	user,
	guest
};

class User
{
	int id;
	std::string name;
	UserState state;
	int workload;
public:
	User();
	User(int id,std::string name,UserState state,int workload);
	int getId() const;
	std::string getName() const;
	UserState getState() const;
	int getWorkload() const;
};

} // namespace rls