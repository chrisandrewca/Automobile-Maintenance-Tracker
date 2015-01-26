#include "Database.hpp"
#include "utils\Debug.h"
using namespace AMT;

Database::Database() :
	sqlite3(nullptr)
{}

Database::~Database()
{
	if (this->sqlite3)
	{
		sqlite3_close_v2(this->sqlite3);
	}
}

bool Database::Open(const std::string& name, std::string& errorMessage)
{
	return this->setup(name.data(), errorMessage);
}

bool Database::setup(const char* databaseName, std::string& errorMessage)
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
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"Name TEXT)",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

	sqlite3_exec(this->sqlite3,
		"CREATE TABLE IF NOT EXISTS MaintenanceType ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"Name TEXT)",
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
			"Type INTEGER REFERENCES VehicleType (ID),"
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
			"Type INTEGER REFERENCES MaintenanceType (ID),"
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
			"MaintenanceTypeID INTEGER REFERENCES MaintenanceType (ID),"
			"PRIMARY KEY (VehicleTypeID, MaintenanceTypeID))",
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
			"ApplicableVehicleType INTEGER REFERENCES VehicleType (ID))",
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

/// Track a new type of vehicle
/// @param type the new type of vehicle
/// @return true if added or already added otherwise false
bool
Database::AddTypeOfVehicle(const std::string& type)
{
	bool succeeded = false;
	// SQL query here
	return succeeded;
}