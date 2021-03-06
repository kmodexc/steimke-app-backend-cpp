#include "User.h"

namespace rls
{
	std::string toString(const UserState &state)
	{
		switch (state)
		{
		case UserState::admin:
			return "admin";
			break;
		case UserState::guest:
			return "guest";
			break;
		case UserState::user:
			return "user";
			break;
		case UserState::mod:
			return "mod";
		default:
			break;
		}
		return "guest";
	}
	UserState parseUserState(const std::string &str)
	{
		if (str == "admin")
		{
			return UserState::admin;
		}
		else if (str == "guest")
		{
			return UserState::guest;
		}
		else if (str == "user")
		{
			return UserState::user;
		}
		else if (str == "mod")
		{
			return UserState::mod;
		}
		return UserState::guest;
	}
	User::User() : User(0, "", "", "", UserState::guest, 0)
	{
	}
	User::User(int id, std::string name, std::string password, std::string email, UserState state, int workload)
	{
		this->id = id;
		this->name = name;
		this->password = password;
		this->email = email;
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
	std::string User::getPassword() const
	{
		return password;
	}
	std::string User::getEmail() const
	{
		return email;
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
