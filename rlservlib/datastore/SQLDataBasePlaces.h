#pragma once
#include "IDataBase.h"
#include <sqlite3.h>
#include <mutex>
#include <fstream>
#include "Place.h"

namespace rls
{

class SQLDataBasePlaces : public IDataBase<Place>
{
	sqlite3* db;
	// threadsafe
	std::mutex mtx;
public:
	SQLDataBasePlaces();
	Place get(int id) override;
	std::vector<Place> getAll() override;
	std::vector<int> getIDs() override;
	void add(Place &) override;
	void update(Place &) override;
	void del(int id) override;
	~SQLDataBasePlaces();

public:
	const std::string db_file_name = "db_places.sqlite";
	const std::string db_file_path = "data/";
};

} // namespace rls