#include "SQLDataBasePlaces.h"
#include "spdlog/spdlog.h"

#define CHECK_SQL_ERROR(returncode, errorRetVal)                                          \
	if (returncode != SQLITE_OK && returncode != SQLITE_DONE && returncode != SQLITE_ROW) \
	{                                                                                     \
		spdlog::get("rlservlib")->error("SQL error({}): {}",returncode,sqlite3_errmsg(db));			      \
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
		// default place table
		const char command[] = "CREATE TABLE Places("
							   "ID 			INT		,"
							   "state		int		,"
							   "name		text	,"
							   "creatorId	int		)";

		char *errmsg;
		if (sqlite3_exec(db, command, 0, nullptr, &errmsg) != SQLITE_OK)
		{
			if (errmsg)
			{
				spdlog::get("rlservlib")->error("error creating table; message={}",errmsg);
			}else{
				spdlog::get("rlservlib")->error("error creating table; nomsg");
			}
			sqlite3_close(db);
			db = nullptr;
			return;
		}

		// create table if db was created
		// place members table
		const char command2[] = "CREATE TABLE PlaceMembers("
								"ID 		INT		,"
								"type		int		,"
								"member		int		)";

		if (sqlite3_exec(db, command2, 0, nullptr, &errmsg) != SQLITE_OK)
		{
			if (errmsg)
			{
				spdlog::get("rlservlib")->error("error creating table; message={}",errmsg);
			}else{
				spdlog::get("rlservlib")->error("error creating table; nomsg");
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
		spdlog::get("rlservlib")->error("Database closed");
		return;
	}
	stdlock lock(mtx);

	const char command[] = "INSERT INTO Places (ID,state,name,creatorId) "
						   "VALUES (?1,?2,?3,?4)";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 1, it.id);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 2, (int)it.type);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, 3, it.name.c_str(), it.name.length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 4, it.creatorId);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, );

	// members only if private

	if (it.type == PlaceType::_private)
	{
		// set members

		const char command2[] = "INSERT INTO PlaceMembers (ID,type,member) "
								"VALUES (?1,?2,?3)";

		for (unsigned int cnt = 0; cnt < it.members.size(); cnt++)
		{
			sqlite3_stmt *stmt;
			int rc = sqlite3_prepare_v2(db, command2, sizeof(command2), &stmt, nullptr);
			CHECK_SQL_ERROR(rc, );

			rc = sqlite3_bind_int(stmt, 1, it.id);
			CHECK_SQL_ERROR(rc, );
			rc = sqlite3_bind_int(stmt, 2, 0);
			CHECK_SQL_ERROR(rc, );
			rc = sqlite3_bind_int(stmt, 3, it.members[cnt]);
			CHECK_SQL_ERROR(rc, );

			rc = sqlite3_step(stmt);
			CHECK_SQL_ERROR(rc, );

			rc = sqlite3_finalize(stmt);
			CHECK_SQL_ERROR(rc, );
		}
	}
}
Place SQLDataBasePlaces::get(int ex_id)
{
	if (db == nullptr)
	{
		spdlog::get("rlservlib")->error("Database closed");
		return Place();
	}
	stdlock lock(mtx);

	const char command[] = "SELECT * FROM Places WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, Place());

	rc = sqlite3_bind_int(stmt, 1, ex_id);
	CHECK_SQL_ERROR(rc, Place());

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, Place());

	// read actively from db to make shure there is really a valid item
	int id = sqlite3_column_int(stmt, 0);
	PlaceType state = (PlaceType)sqlite3_column_int(stmt, 1);
	std::string name = TO_CPP_STRING(sqlite3_column_text(stmt, 2));
	int creatorId = sqlite3_column_int(stmt, 3);

	Place it;
	it.id = id;
	it.name = name;
	it.type = state;
	it.creatorId = creatorId;

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, Place());

	// get members only if private

	if (it.type == PlaceType::_private)
	{
		// set members

		const char command2[] = "SELECT * FROM PlaceMembers WHERE ID=?1";

		stmt = nullptr;
		int rc = sqlite3_prepare_v2(db, command2, sizeof(command2), &stmt, nullptr);
		CHECK_SQL_ERROR(rc, Place());

		rc = sqlite3_bind_int(stmt, 1, it.id);
		CHECK_SQL_ERROR(rc, Place());

		rc = sqlite3_step(stmt);
		CHECK_SQL_ERROR(rc, Place());
		while (rc == SQLITE_ROW)
		{
			it.members.push_back(sqlite3_column_int(stmt, 2));
			rc = sqlite3_step(stmt);
			CHECK_SQL_ERROR(rc, Place());
		}

		rc = sqlite3_finalize(stmt);
		CHECK_SQL_ERROR(rc, Place());
	}

	return it;
}
std::vector<int> SQLDataBasePlaces::getIDs()
{
	if (db == nullptr)
	{
		spdlog::get("rlservlib")->error("Database closed");
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
	spdlog::get("rlservlib")->debug("Found {} Places in DB" , retval.size());
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
	spdlog::get("rlservlib")->debug("Return {} Places",retval.size());
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
		spdlog::get("rlservlib")->error("Database closed");
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

	char command2[] = "DELETE FROM PlaceMembers WHERE ID=?1";
	stmt = nullptr;
	rc = sqlite3_prepare_v2(db, command2, sizeof(command2), &stmt, nullptr);
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
