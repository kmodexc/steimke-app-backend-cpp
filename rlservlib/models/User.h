#pragma once
#include <string>

namespace rls
{

enum class UserState
{
	admin,
	user,
	guest,
	mod
};

std::string toString(const UserState &state);
UserState parseUserState(const std::string &str);

class User
{
	int id;
	std::string name;
	std::string password;
	std::string email;
	UserState state;
	int workload;
public:
	User();
	User(int id,std::string name,std::string password,std::string email,UserState state,int workload);
	int getId() const;
	std::string getName() const;
	std::string getPassword() const;
	std::string getEmail() const;
	UserState getState() const;
	int getWorkload() const;
};

} // namespace rls