#include "Database.hpp"
#include "utils\Debug.h"
#include <cassert>
using namespace AMT;

#define AMT_SQL_QUERY_BAG_INITIAL_SIZE 50

Database::Database() :
	sqlQueryBindIndices(),
	sqlQueryResultColumnIndices(),
	sqlitePreparedStatements(),
	sqlQueryBag(),
	sqlite3(nullptr)
{
	sqlitePreparedStatements.reserve(AMT_SQL_QUERY_BAG_INITIAL_SIZE);
	sqlQueryBag.reserve(AMT_SQL_QUERY_BAG_INITIAL_SIZE);

	// n: users
	// 0: AddTypeOfVehicle
	sqlQueryBag.push_back("INSERT INTO VehicleType (Name) VALUES (?)");
	sqlQueryBindIndices[sqlQueryBag[0]] = std::unordered_map<std::string, int>(
	{
		{ "name", 1 }
	});

	// 1: UpdateTypesOfVehicles
	sqlQueryBag.push_back("UPDATE VehicleType SET Name=? WHERE Name=?");
	sqlQueryBindIndices[sqlQueryBag[1]] = std::unordered_map<std::string, int>(
	{
		{ "newName", 1 }, { "name", 2 }
	});

	// 2: ListAllTypesOfVehicles
	sqlQueryBag.push_back("SELECT * FROM VehicleType");
	sqlQueryResultColumnIndices[sqlQueryBag[2]] = std::unordered_map<std::string, int>(
	{
		{ "name", 0 }
	});

	assert(sqlQueryBag.size() < AMT_SQL_QUERY_BAG_INITIAL_SIZE);
}

Database::~Database()
{
	for (auto& statement : sqlitePreparedStatements)
	{
		if (statement.second)
		{
			sqlite3_finalize(statement.second);
		}
	}

	if (this->sqlite3)
	{
		sqlite3_close_v2(this->sqlite3);
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
			"Type TEXT REFERENCES VehicleType (Name) ON UPDATE CASCADE,"
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
			"Type TEXT REFERENCES MaintenanceType (Name) ON UPDATE CASCADE,"
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
			"VehicleType TEXT REFERENCES VehicleType (Name) ON UPDATE CASCADE,"
			"MaintenanceType TEXT REFERENCES MaintenanceType (Name) ON UPDATE CASCADE,"
			"PRIMARY KEY (VehicleType, MaintenanceType))",
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
			"ApplicableVehicleType TEXT REFERENCES VehicleType (Name) ON UPDATE CASCADE)",
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

	for (std::size_t i = 0; i < sqlQueryBag.size(); i++)
	{
		/// !!! TODO ERROR CHECKING LOGGING
		sqlite3_stmt* queryStmt;
		int prepResult = sqlite3_prepare_v2(this->sqlite3,
			sqlQueryBag[i].data(),
			sqlQueryBag[i].size(),
			&queryStmt,
			NULL);
		std::cout << "sqlite_prepare_v2 " << sqlQueryBag[i] << ": " << prepResult;
		
		sqlitePreparedStatements[sqlQueryBag[i]] = queryStmt;
	}

	return (errorMessage.size() == 0);
}

bool
Database::AddTypeOfVehicle(const std::string& name)
{
	bool succeeded = false;

	std::size_t sqlQueryBagStatementIndex = 0;
	const std::string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
	SQLitePreparedStatementPtr statement = sqlitePreparedStatements[sqlQueryString];
	SQLiteBindIndices bindIndices = sqlQueryBindIndices[sqlQueryString];

	/// !!! important
	// need mutex on SQLitePreparedStatementPtr
	// for functions which Bind + Step
		// API, Datastore errors
			// std::timed_mutex - succeed || timeout
			// support C++ library & http nicely

	/// !!! TODO ERROR CHECKING LOGGING
	int bindResult = sqlite3_bind_text(statement,
		bindIndices["name"],
		name.data(),
		name.size(),
		NULL);
	std::cout << "AddTypeOfVehicle sqlite3_bind_text: " << bindResult << "\n";

	/// !!! TODO ERROR CHECKING LOGGING
	int stepResult = sqlite3_step(statement);
	std::cout << "AddTypeOfVehicle sqlite3_step: " << stepResult << "\n";

	sqlite3_reset(statement);
	sqlite3_clear_bindings(statement);

	return succeeded;
}

bool
Database::UpdateTypesOfVehicles(const std::string& name, const std::string& newName)
{
	/// !!! important
	// need mutex on SQLitePreparedStatementPtr
	// for functions which Bind + Step
	// API, Datastore errors
	// std::timed_mutex - succeed || timeout
	// support C++ library & http nicely

	bool succeeded = false;

	std::size_t sqlQueryBagStatementIndex = 1;
	const string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
	SQLitePreparedStatementPtr statement = sqlitePreparedStatements[sqlQueryString];
	SQLiteBindIndices bindIndices = sqlQueryBindIndices[sqlQueryString];

	/// !!! TODO ERROR CHECKING LOGGING
	int bindResult = sqlite3_bind_text(statement,
		bindIndices["name"],
		name.data(),
		name.size(),
		NULL);
	std::cout << "UpdateTypesOfVehicles sqlite3_bind_text: " << bindResult << "\n";

	bindResult = sqlite3_bind_text(statement,
		bindIndices["newName"],
		newName.data(),
		newName.size(),
		NULL);
	std::cout << "UpdateTypesOfVehicles sqlite3_bind_text: " << bindResult << "\n";

	/// !!! TODO ERROR CHECKING LOGGING
	int stepResult = sqlite3_step(statement);
	std::cout << "UpdateTypesOfVehicles sqlite3_step: " << stepResult << "\n";

	sqlite3_reset(statement);
	sqlite3_clear_bindings(statement);

	return succeeded;
}

std::unique_ptr<std::vector<std::string> >
Database::ListAllTypesOfVehicles()
{
	/// !!! TODO ERROR CHECKING LOGGING
	/// !!! important
	// need mutex on SQLitePreparedStatementPtr
	// for functions which Bind, Step
	// API, Datastore errors
	// std::timed_mutex - succeed || timeout
	// support C++ library & http nicely

	auto* allVehicleTypes = new std::vector<string>();

	std::size_t sqlQueryBagStatementIndex = 2;
	const string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
	SQLitePreparedStatementPtr statement = sqlitePreparedStatements[sqlQueryString];
	SQLiteColumnIndices columnIndices = sqlQueryResultColumnIndices[sqlQueryString];

	int stepResult = sqlite3_step(statement);
	std::cout << "ListAllTypesOfVehicles sqlite3_step: " << stepResult << "\n";

	while (SQLITE_ROW == stepResult)
	{
		ustring nameBytes = sqlite3_column_text(statement, columnIndices["name"]);

		string name(nameBytes.begin(), nameBytes.end());
		allVehicleTypes->push_back(name);

		stepResult = sqlite3_step(statement);
		std::cout << "ListAllTypesOfVehicles sqlite3_step: " << stepResult << "\n";
	}

	sqlite3_reset(statement);

	return std::unique_ptr<std::vector<string> >(allVehicleTypes);
}