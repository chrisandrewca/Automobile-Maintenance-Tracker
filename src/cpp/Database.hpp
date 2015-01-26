#ifndef _AMT_DATABASE_HPP_
#define _AMT_DATABASE_HPP_

#include "DataStore.hpp"
#include "sqlite\sqlite3.h"
#include <string>
#include <unordered_map>
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
	/// @param name the new type of vehicle
	/// @return true if added or already added otherwise false
	bool
	AddTypeOfVehicle(const utf8string& name) override;

	/// Update the name of a vehicle type
	/// @param name the current name
	/// @param newName the new name of the vehicle type
	/// @return true if updated or the same otherwise false
	bool
	UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName) override;

	/// List available types of vehicles
	/// @return the list of available vehicles
	std::unique_ptr<std::vector<utf8string> >
	ListAllTypesOfVehicles() override;

private:
	template<typename K, typename V>
	using u_map = std::unordered_map<K, V>;
	
	typedef std::string string;
	typedef std::basic_string <unsigned char> ustring;

	typedef sqlite3_stmt* SQLitePreparedStatementPtr;
	typedef u_map<string, int> SQLiteBindIndices;
	typedef u_map<string, int> SQLiteColumnIndices;

	u_map<string, SQLitePreparedStatementPtr> sqlitePreparedStatements;
	u_map<string, SQLiteBindIndices> sqlQueryBindIndices;
	u_map<string, SQLiteColumnIndices> sqlQueryResultColumnIndices;
	std::vector<string> sqlQueryBag;
	sqlite3* sqlite3;

	bool Setup(const char* databaseName, std::string& errorMessage);
};

}

#endif