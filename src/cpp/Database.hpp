#ifndef _AMT_DATABASE_HPP_
#define _AMT_DATABASE_HPP_

#include "DataStore.hpp"
#include "sqlite\sqlite3.h"
#include <string>
#include <unordered_map>;
#include <memory>

namespace AMT
{

class Database : public DataStore
{
public:
	Database();
	~Database();

	bool Open(const std::string& name, std::string& errorMessage) override;

	/// Track a new type of vehicle
	/// @param type the new type of vehicle
	/// @return true if added or already added otherwise false
	bool
	AddTypeOfVehicle(const std::string& name) override;

private:
	struct SQLitePreparedStatement
	{
		sqlite3_stmt* statement;
		std::unordered_map<std::string, int>* bindIndices;
	};

	typedef std::unordered_map<std::string, int> SQLiteBindIndices;

	std::unordered_map<std::string, SQLitePreparedStatement> sqlPreparedStatements;
	std::unordered_map<std::string, SQLiteBindIndices> sqlQueryBindIndices;
	std::vector<std::string> sqlQueryBag;
	sqlite3* sqlite3;

	virtual bool Setup(const char* databaseName, std::string& errorMessage);
};

}

#endif