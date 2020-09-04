#pragma once
#include "IDataBase.h"
#include "User.h"
#include <sqlite3.h>
#include <mutex>
#include <fstream>

namespace rls
{

class SQLDataBaseUser : public IDataBase<User>
{
	sqlite3* db;
	// threadsafe
	std::mutex mtx;
public:
	SQLDataBaseUser();
	User get(int id) override;
	std::vector<User> getAll() override;
	std::vector<int> getIDs() override;
	void add(User &) override;
	void update(User &) override;
	void del(int id) override;
	~SQLDataBaseUser();

public:
	const std::string db_file_name = "db_user.sqlite";
	const std::string db_file_path = "";
};

} // namespace rls