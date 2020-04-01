#include "SQLDataBasePlaces.h"

#define CHECK_SQL_ERROR(returncode, errorRetVal)                                          \
	if (returncode != SQLITE_OK && returncode != SQLITE_DONE && returncode != SQLITE_ROW) \
	{                                                                                     \
		cerr << "SQL error(" << returncode << "): " << sqlite3_errmsg(db) << endl;        \
		sqlite3_close(db);                                                                \
		db = nullptr;                                                                     \
		return errorRetVal;                                                               \
	}

#define TO_CPP_STRING(in) (in == nullptr ? std::string("null") : (const char *)in)

using namespace std;

namespace rls
{

typedef std::unique_lock<std::mutex> stdlock;

SQLDataBasePlaces::SQLDataBasePlaces()
{
	stdlock lock(mtx);
	std::string fqn = db_file_path + db_file_name;

	// check if db already exists
	bool db_exist = false;
	ifstream dbfstr(fqn);
	if (dbfstr.good())
		db_exist = true;
	dbfstr.close();

	// open db
	int rc = sqlite3_open(fqn.c_str(), &db);
	CHECK_SQL_ERROR(rc, );

	if (!db_exist)
	{
		// create table if db was created
		const char command[] = "CREATE TABLE Places("
							   "ID 			INT		,"
							   "state		int		,"
							   "name		text	)";

		char *errmsg;
		if (sqlite3_exec(db, command, 0, nullptr, &errmsg) != SQLITE_OK)
		{
			cerr << "error creating table" << endl;
			if (errmsg)
			{
				cerr << "errormessage: " << errmsg << endl;
			}
			sqlite3_close(db);
			db = nullptr;
			return;
		}
	}
}
void SQLDataBasePlaces::add(Place &it)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return;
	}
	stdlock lock(mtx);

	const char command[] = "INSERT INTO Places (ID,state,name) "
						   "VALUES (?1,?2,?3)";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 1, it.id);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 2, (int)it.type);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, 3, it.name.c_str(), it.name.length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, );
}
Place SQLDataBasePlaces::get(int id)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return Place();
	}
	stdlock lock(mtx);

	const char command[] = "SELECT * FROM Places WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, Place());

	rc = sqlite3_bind_int(stmt, 1, id);
	CHECK_SQL_ERROR(rc, Place());

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, Place());

	PlaceType state = (PlaceType)sqlite3_column_int(stmt, 1);
	std::string name = TO_CPP_STRING(sqlite3_column_text(stmt, 2));

	Place it;
	it.id = id;
	it.name = name;
	it.type = state;

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, Place());

	return it;
}
std::vector<int> SQLDataBasePlaces::getIDs()
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return std::vector<int>();
	}
	stdlock lock(mtx);

	char command[] = "SELECT ID FROM Places";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, std::vector<int>());

	std::vector<int> retval;

	do
	{
		CHECK_SQL_ERROR(rc, std::vector<int>());
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW)
			retval.push_back(sqlite3_column_int(stmt, 0));
	} while (rc != SQLITE_DONE);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, std::vector<int>());
	cout << "Found " << retval.size() << " Place in DB" << endl;
	return retval;
}
std::vector<Place> SQLDataBasePlaces::getAll()
{
	// dont invoke mutex here
	auto ids = getIDs();

	std::vector<Place> retval;

	for (auto id : ids)
	{
		retval.push_back(get(id));
	}
	cout << "Return " << retval.size() << " Place" << endl;
	return retval;
}
void SQLDataBasePlaces::update(Place &it)
{
	// dont invoke mutex here
	del(it.id);
	add(it);
}
void SQLDataBasePlaces::del(int id)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return;
	}
	stdlock lock(mtx);

	char command[] = "DELETE FROM Places WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 1, id);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, );
}
SQLDataBasePlaces::~SQLDataBasePlaces()
{
	mtx.try_lock(); // dont really lock here to try safe db in case of exception
	if (db != nullptr)
		sqlite3_close(db);
}

} // namespace rls