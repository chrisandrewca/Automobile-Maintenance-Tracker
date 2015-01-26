#include "Database.hpp"
#include "utils\Debug.h"
#include <cassert>
using namespace AMT;

#define SQL_QUERY_BAG_INITIAL_SIZE 50

Database::Database() :
	sqlPreparedStatements(SQL_QUERY_BAG_INITIAL_SIZE),
	sqlQueryBag(SQL_QUERY_BAG_INITIAL_SIZE),
	sqlite3(nullptr)
{
	// 0
	sqlQueryBag.push_back("INSERT INTO VehicleType (Name) VALUES (?)");
	sqlQueryBindIndices[sqlQueryBag[0]] = std::unordered_map<std::string, int>(
	{
		{ "name", 0 }
	});
	// 1

	assert(sqlQueryBag.size() < SQL_QUERY_BAG_INITIAL_SIZE);

	for (std::size_t i = 0; i < sqlQueryBag.size(); i++)
	{
		/// !!! TODO ERROR CHECKING
		sqlite3_stmt* queryStmt;
		sqlite3_prepare_v2(this->sqlite3,
			sqlQueryBag[i].data(),
			sqlQueryBag[i].size(),
			&queryStmt,
			NULL);

		sqlPreparedStatements[sqlQueryBag[i]] = queryStmt;
	}
}

Database::~Database()
{
	if (this->sqlite3)
	{
		sqlite3_close_v2(this->sqlite3);
	}

	for (auto& statement : sqlPreparedStatements)
	{
		if (statement.second)
		{
			sqlite3_finalize(statement.second);
		}
	}
}

bool Database::Open(const std::string& name, std::string& errorMessage)
{
	return this->Setup(name.data(), errorMessage);
}

bool Database::Setup(const char* databaseName, std::string& errorMessage)
{
	errorMessage.clear();

	if (sqlite3_open_v2(databaseName,
						&sqlite3,
						SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
						NULL)
		!= SQLITE_OK)
	{
		// set error string
	}

	char* errMsg = nullptr;
	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS VehicleType ("
			"Name TEXT PRIMARY KEY)",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS MaintenanceType ("
			"Name TEXT PRIMARY KEY)",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS Vehicle ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"Type TEXT REFERENCES VehicleType (Name),"
			"Make TEXT,"
			"Model TEXT,"
			"Year INTEGER,"
			"Odometer INTEGER)",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS Maintenance ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"VehicleID INTEGER REFERENCES Vehicle (ID),"
			"Type TEXT REFERENCES MaintenanceType (Name),"
			"Date INTEGER)",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS VehicleApplicableMaintenance ("
			"VehicleTypeID INTEGER REFERENCES VehicleType (ID),"
			"MaintenanceType TEXT REFERENCES MaintenanceType (Name),"
			"PRIMARY KEY (VehicleTypeID, MaintenanceType))",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	//sqlite3_exec(sqlite,
	//	"ALTER TABLE Vehicle ADD COLUMN Color TEXT",
	//	NULL, NULL, &errMsg);
	//if (errMsg)
	//{
	//	std::cout << errMsg << std::endl;
	//	sqlite3_free(errMsg);
	//	errMsg = nullptr;
	//}

	//sqlite3_exec(sqlite,
	//	"BEGIN TRANSACTION;"
	//	"CREATE TEMPORARY TABLE backup_Vehicle ("
	//	"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	//	"Type INTEGER REFERENCES VehicleType (ID),"
	//	"Model TEXT,"
	//	"Year INTEGER,"
	//	"Odometer INTEGER);"
	//	"INSERT INTO backup_Vehicle SELECT (Type, Model, Year, Odometer) FROM Vehicle;"
	//	"DROP TABLE Vehicle",
	//	NULL, NULL, &errMsg);
	//if (errMsg)
	//{
	//	std::cout << errMsg << std::endl;
	//	sqlite3_free(errMsg);
	//	errMsg = nullptr;
	//}

	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS VehicleUserDefinedField ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"Name TEXT,"
			"ApplicableVehicleType TEXT REFERENCES VehicleType (Name))",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS VehicleUserDefinedFieldData ("
			"VehicleID INTEGER REFERENCES Vehicle (ID),"
			"UserDefinedFieldID INTEGER REFERENCES VehicleUserDefinedField (ID),"
			"Value TEXT,"
			"PRIMARY KEY (VehicleID, UserDefinedFieldID))",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	return (errorMessage.size() == 0);
}

bool
Database::AddTypeOfVehicle(const std::string& name)
{
	bool succeeded = false;

	std::size_t sqlQueryBagStatementIndex = 0;
	const std::string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
	SQLitePreparedStatementPtr statement = sqlPreparedStatements[sqlQueryString];
	SQLiteBindIndices bindIndices = sqlQueryBindIndices[sqlQueryString];

	/// !!! TODO ERROR CHECKING
	sqlite3_bind_text(statement,
		bindIndices["name"],
		name.data(),
		name.size(),
		NULL);

	// need mutex on SQLITEPreparedStatement
		// for functions which Bind + Step
		// API, Datastore errors
			// std::timed_mutex - succeed || timeout
				// support C++ library & http nicely

	/// !!! TODO ERROR CHECKING
	sqlite3_step(statement);



	return succeeded;
}