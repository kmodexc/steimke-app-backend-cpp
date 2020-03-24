#pragma once
#include "IDataBase.h"
#include <sqlite3.h>
#include <iostream>
#include <mutex>

namespace rls{

class SQLDataBase : public IDataBase{
	// private sql varialbes
private:
	sqlite3* db;
	// threadsafe
	std::mutex mtx;
public:
	SQLDataBase();
	void addItem(Item);
	Item getItem(int id) override;
	std::vector<int> getIDs();
	std::vector<Item> getItems() override;
	void updateItem(Item);
	void deleteItem(int id);
	~SQLDataBase() override;

public:
	const std::string db_file_name = "db1.sqlite";
	const std::string db_file_path = "";
};


}