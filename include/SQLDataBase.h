#pragma once
#include "IDataBase.h"
#include <sqlite3.h>
#include <iostream>

namespace rls{

class SQLDataBase : public IDataBase{
	// private sql varialbes
private:
	sqlite3* db;

	// protected helper functions
protected:
	// execute sql command
	bool execute(std::string command);
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);

public:
	SQLDataBase();
	Item getItem(int id) override;
	std::vector<Item> getItems() override;
	~SQLDataBase() override;

public:
	const std::string db_file_name = "db1.sqlite";
	const std::string db_file_path = "";
};


}