#include "SQLDataBaseUser.h"
#include "spdlog/spdlog.h"

#define CHECK_SQL_ERROR(returncode, errorRetVal)                                          \
	if (returncode != SQLITE_OK && returncode != SQLITE_DONE && returncode != SQLITE_ROW) \
	{                                                                                     \
		spdlog::error("SQL error({}): {}",returncode,sqlite3_errmsg(db));			      \
		sqlite3_close(db);                                                                \
		db = nullptr;                                                                     \
		return errorRetVal;                                                               \
	}

#define TO_CPP_STRING(in) (in == nullptr ? std::string("null") : (const char *)in)

using namespace std;

namespace rls
{

typedef std::unique_lock<std::mutex> stdlock;

SQLDataBaseUser::SQLDataBaseUser()
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
		const char command[] = "CREATE TABLE Users("
							   "ID 			INT		,"
							   "state		int		,"
							   "name		text	,"
							   "pw			text	,"
							   "email		text	,"
							   "wl			int		)";

		char *errmsg;
		if (sqlite3_exec(db, command, 0, nullptr, &errmsg) != SQLITE_OK)
		{
			if (errmsg)
			{
				spdlog::error("error creating table; message={}",errmsg);
			}else{
				spdlog::error("error creating table; nomsg");
			}
			sqlite3_close(db);
			db = nullptr;
			return;
		}
	}
}
void SQLDataBaseUser::add(User &it)
{
	if (db == nullptr)
	{
		spdlog::error("Database closed");
		return;
	}
	stdlock lock(mtx);

	const char command[] = "INSERT INTO Users (ID,state,name,pw,email,wl) "
						   "VALUES (@ID,@state,@name,@pw,@email,@wl)";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt,"@ID"), it.getId());
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt,"@state"), (int)it.getState());
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt,"@name"), it.getName().c_str(), it.getName().length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt,"@pw"), it.getPassword().c_str(), it.getPassword().length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt,"@email"), it.getEmail().c_str(), it.getEmail().length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt,"@wl"), it.getWorkload());
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, );
}
User SQLDataBaseUser::get(int id)
{
	if (db == nullptr)
	{
		spdlog::error("Database closed");
		return User();
	}
	stdlock lock(mtx);

	const char command[] = "SELECT * FROM Users WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, User());

	rc = sqlite3_bind_int(stmt, 1, id);
	CHECK_SQL_ERROR(rc, User());

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, User());

	id = sqlite3_column_int(stmt, 0);
	UserState state = (UserState)sqlite3_column_int(stmt, 1);
	std::string name = TO_CPP_STRING(sqlite3_column_text(stmt, 2));
	std::string pw = TO_CPP_STRING(sqlite3_column_text(stmt, 3));
	std::string email = TO_CPP_STRING(sqlite3_column_text(stmt, 4));
	int wl = sqlite3_column_int(stmt, 5);

	User it(id, name,pw,email, state, wl);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, User());

	return it;
}
std::vector<int> SQLDataBaseUser::getIDs()
{
	if (db == nullptr)
	{
		spdlog::error("Database closed");
		return std::vector<int>();
	}
	stdlock lock(mtx);

	char command[] = "SELECT ID FROM Users";
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
	spdlog::debug("Found {} Users in DB" , retval.size());
	return retval;
}
std::vector<User> SQLDataBaseUser::getAll()
{
	// dont invoke mutex here
	auto ids = getIDs();

	std::vector<User> retval;

	for (auto id : ids)
	{
		retval.push_back(get(id));
	}
	spdlog::debug("Return {} Users" , retval.size());
	return retval;
}
void SQLDataBaseUser::update(User &it)
{
	// dont invoke mutex here
	del(it.getId());
	add(it);
}
void SQLDataBaseUser::del(int id)
{
	if (db == nullptr)
	{
		spdlog::error("Database closed");
		return;
	}
	stdlock lock(mtx);

	char command[] = "DELETE FROM Users WHERE ID=?1";
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
SQLDataBaseUser::~SQLDataBaseUser()
{
	mtx.try_lock(); // dont really lock here to try safe db in case of exception
	if (db != nullptr)
		sqlite3_close(db);
}

} // namespace rls