#pragma once
#include "IDataBase.h"
#include <sqlite3.h>
#include <iostream>
#include <mutex>
#include <fstream>

namespace rls{

class SQLDataBase : public IDataBase{
	// private sql varialbes
private:
	sqlite3* db;
	// threadsafe
	std::mutex mtx;
public:
	SQLDataBase();
	void addItem(Item) override;
	Item getItem(int id) override;
	std::vector<int> getIDs() override;
	std::vector<Item> getItems() override;
	void updateItem(Item) override;
	void deleteItem(int id) override;
	~SQLDataBase() override;

public:
	const std::string db_file_name = "db1.sqlite";
	const std::string db_file_path = "";
};


}