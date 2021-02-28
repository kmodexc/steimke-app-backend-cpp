#pragma once
#include "IDataBase.h"
#include <sqlite3.h>
#include <mutex>
#include <fstream>

namespace rls{

class SQLDataBaseItem : public IDataBase<Item>{
	// private sql varialbes
private:
	sqlite3* db;
	// threadsafe
	std::mutex mtx;
public:
	SQLDataBaseItem();
	void add(Item&) override;
	Item get(int id) override;
	std::vector<int> getIDs() override;
	std::vector<Item> getAll() override;
	void update(Item&) override;
	void del(int id) override;
	~SQLDataBaseItem() override;

public:
	const std::string db_file_name = "db_item.sqlite";
	const std::string db_file_path = "data/";
};


}