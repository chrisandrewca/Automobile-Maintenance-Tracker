#include "Database.hpp"
#include "utils/Debug.h"
#include <cassert>
using namespace AMT;

#define AMT_SQL_QUERY_BAG_INITIAL_SIZE 50

Database::Database() :
    sqlite(nullptr),
    sqlQueryBag(),
	sqlQueryBindIndices(),
    sqlQueryPreparedStatements(),
    sqlQueryResultColumnIndices()
{
    sqlQueryPreparedStatements.reserve(AMT_SQL_QUERY_BAG_INITIAL_SIZE);
	sqlQueryBag.reserve(AMT_SQL_QUERY_BAG_INITIAL_SIZE);

	// n: users
	// 0: AddTypeOfVehicle
	sqlQueryBag.push_back("INSERT INTO VehicleType (Name) VALUES (?)");
    sqlQueryBindIndices[sqlQueryBag[0]] = u_map<string, int>
    ({
		{ "name", 1 }
	});

	// 1: UpdateTypesOfVehicles
	sqlQueryBag.push_back("UPDATE VehicleType SET Name=? WHERE Name=?");
    sqlQueryBindIndices[sqlQueryBag[1]] = u_map<string, int>
    ({
		{ "newName", 1 }, { "name", 2 }
	});

	// 2: ListAllTypesOfVehicles
	sqlQueryBag.push_back("SELECT * FROM VehicleType");
    sqlQueryResultColumnIndices[sqlQueryBag[2]] = u_map<string, int>
    ({
		{ "name", 0 }
	});

    // 3: CreateVehicle
    sqlQueryBag.push_back("INSERT INTO Vehicle (ID) VALUES (NULL)");

    // 4: DeleteVehicle
    sqlQueryBag.push_back("DELETE FROM Vehicle WHERE ID=?");
    sqlQueryBindIndices[sqlQueryBag[4]] = u_map<string, int>
    ({
        { "id", 1 }
    });

    // 5: ListAllVehicles
    sqlQueryBag.push_back("SELECT * FROM Vehicle");
    sqlQueryResultColumnIndices[sqlQueryBag[5]] = u_map<string, int>
    ({
        { "id", 0 }, { "type", 1 }, { "make", 2 }, { "model", 3 }, { "year", 4 },
        { "odometer", 5 }
    });


    // 6: ListAllVehicles
    sqlQueryBag.push_back("SELECT (Name) FROM VehicleUserDefinedField WHERE "
                          "ApplicableVehicleType=?");
    sqlQueryBindIndices[sqlQueryBag[6]] = u_map<string, int>
    ({
        { "vehicleType", 1 }
    });
    sqlQueryResultColumnIndices[sqlQueryBag[6]] = u_map<string, int>
    ({
        { "fieldName", 0 }
    });

    // 7: ListAllVehicles
    // using PK in this query gives us quick access but repeated sqlite_step/clear_bind?/bind
    sqlQueryBag.push_back("SELECT (Value) FROM VehicleUserDefinedFieldValue WHERE "
                          "VehicleID=? AND UserDefinedFieldName=?");
    sqlQueryBindIndices[sqlQueryBag[7]] = u_map<string, int>
    ({
         // clunky way to describe SQL info, column identifiers repeated and inconsistent
         // "this is my sql statement (generally unqiueish)"
         // "these are the binds (generally uniqueish)"
         // "these are the columns I'm accessing in this query (unique to statement)
                // so this way of describing info is not bad we just need a:
                    // "this is what I call the column in my program regardless of location"
                        // const string tableColName
                        // Col names are unique per table
                            // can use in SQL query and Map
        { "vehicleId", 1 }, { "fieldName", 2 }
    });
    sqlQueryResultColumnIndices[sqlQueryBag[7]] = u_map<string, int>
    ({
        { "fieldValue", 0 }
    });

	assert(sqlQueryBag.size() < AMT_SQL_QUERY_BAG_INITIAL_SIZE);
}

Database::~Database()
{
    for (auto& statement : sqlQueryPreparedStatements)
	{
		if (statement.second)
		{
			sqlite3_finalize(statement.second);
		}
	}

    if (this->sqlite)
	{
        sqlite3_close_v2(this->sqlite);
	}
}

bool Database::Open(const string& name, string& errorMessage)
{
	return this->Setup(name.data(), errorMessage);
}

bool Database::Setup(const char* databaseName, string& errorMessage)
{
	errorMessage.clear();

	if (sqlite3_open_v2(databaseName,
                        &sqlite,
						SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
						NULL)
		!= SQLITE_OK)
	{
		// set error string
	}

	char* errMsg = nullptr;
    sqlite3_exec(this->sqlite,
		"CREATE TABLE IF NOT EXISTS VehicleType ("
			"Name TEXT PRIMARY KEY)",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

    sqlite3_exec(this->sqlite,
		"CREATE TABLE IF NOT EXISTS MaintenanceType ("
			"Name TEXT PRIMARY KEY)",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

    sqlite3_exec(this->sqlite,
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

    sqlite3_exec(this->sqlite,
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

    sqlite3_exec(this->sqlite,
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

    sqlite3_exec(this->sqlite,
		"CREATE TABLE IF NOT EXISTS VehicleUserDefinedField ("
            //"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"Name TEXT,"
            "ApplicableVehicleType TEXT REFERENCES VehicleType (Name) ON UPDATE CASCADE,"
            "PRIMARY KEY (Name, ApplicableVehicleType))",
		NULL, NULL, &errMsg);
	if (errMsg)
	{
		errorMessage.append(errMsg);
		sqlite3_free(errMsg);
		errMsg = nullptr;
	}

    sqlite3_exec(this->sqlite,
        "CREATE TABLE IF NOT EXISTS VehicleUserDefinedFieldValue ("
			"VehicleID INTEGER REFERENCES Vehicle (ID),"
            "UserDefinedFieldName TEXT REFERENCES VehicleUserDefinedField (Name) ON UPDATE CASCADE,"
			"Value TEXT,"
            "PRIMARY KEY (VehicleID, UserDefinedFieldName))",
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
        int prepResult = sqlite3_prepare_v2(this->sqlite,
                            sqlQueryBag[i].data(),
                            sqlQueryBag[i].size(),
                            &queryStmt,
                            NULL);
		std::cout << "sqlite_prepare_v2 " << sqlQueryBag[i] << ": " << prepResult;
		
        sqlQueryPreparedStatements[sqlQueryBag[i]] = queryStmt;
	}

	return (errorMessage.size() == 0);
}

bool
Database::AddTypeOfVehicle(const utf8string& name)
{
	bool succeeded = false;

	std::size_t sqlQueryBagStatementIndex = 0;
	const string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
    SQLitePreparedStatementPtr statement = sqlQueryPreparedStatements[sqlQueryString];
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
Database::UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName)
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
    SQLitePreparedStatementPtr statement = sqlQueryPreparedStatements[sqlQueryString];
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

std::unique_ptr<std::vector<utf8string> >
Database::ListAllTypesOfVehicles()
{
	/// !!! TODO ERROR CHECKING LOGGING
	/// !!! important
	// need mutex on SQLitePreparedStatementPtr
	// for functions which Bind, Step
	// API, Datastore errors
	// std::timed_mutex - succeed || timeout
	// support C++ library & http nicely

	auto* allVehicleTypes = new std::vector<utf8string>();

	std::size_t sqlQueryBagStatementIndex = 2;
	const string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
    SQLitePreparedStatementPtr statement = sqlQueryPreparedStatements[sqlQueryString];
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

	return std::unique_ptr<std::vector<utf8string> >(allVehicleTypes);
}

std::unique_ptr<Vehicle>
Database::CreateVehicle()
{
    /// !!! TODO ERROR CHECKING LOGGING
    /// !!! important
    // need mutex on SQLitePreparedStatementPtr
    // for functions which Bind, Step
    // API, Datastore errors
    // std::timed_mutex - succeed || timeout
    // support C++ library & http nicely

    std::size_t sqlQueryBagStatementIndex = 3;
    const string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
    SQLitePreparedStatementPtr statement = sqlQueryPreparedStatements[sqlQueryString];

    int stepResult = sqlite3_step(statement);
    std::cout << "CreateVehicle sqlite3_step: " << stepResult << "\n";
    sqlite3_reset(statement);

    sqlite3_int64 objectId = sqlite3_last_insert_rowid(this->sqlite);

    auto* vehicle = new Vehicle(objectId);
    return std::unique_ptr<Vehicle>(vehicle);
}

bool
Database::DeleteVehicle(Vehicle& vehicle)
{
    /// !!! TODO ERROR CHECKING LOGGING
    /// !!! important
    // need mutex on SQLitePreparedStatementPtr
    // for functions which Bind, Step
    // API, Datastore errors
    // std::timed_mutex - succeed || timeout
    // support C++ library & http nicely

    std::size_t sqlQueryBagStatementIndex = 4;
    const string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
    SQLitePreparedStatementPtr statement = this->sqlQueryPreparedStatements[sqlQueryString];
    SQLiteBindIndices bindIndices = this->sqlQueryBindIndices[sqlQueryString];

    int bindResult = sqlite3_bind_int(statement,
                      bindIndices["id"],
                      vehicle.GetID());
    std::cout << "DeleteVehicle sqlite3_bind_int: " << bindResult;

    int stepResult = sqlite3_step(statement);
    std::cout << "CreateVehicle sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(statement);
    sqlite3_clear_bindings(statement);

    return (SQLITE_DONE == stepResult);
}

/// List all available vehicles
/// @return the list of all available vehicles
std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
Database::ListAllVehicles()
{
    /// !!! TODO ERROR CHECKING LOGGING
    /// !!! important
    // need mutex on SQLitePreparedStatementPtr
    // for functions which Bind, Step
    // API, Datastore errors
    // std::timed_mutex - succeed || timeout
    // support C++ library & http nicely

    auto* vehicles = new std::vector<std::unique_ptr<Vehicle>>;

    std::size_t sqlQueryBagStatementIndex = 5;
    string& sqlQueryString = this->sqlQueryBag[sqlQueryBagStatementIndex];
    SQLitePreparedStatementPtr statement = this->sqlQueryPreparedStatements[sqlQueryString];
    SQLiteColumnIndices columnIndices = this->sqlQueryResultColumnIndices[sqlQueryString];

    int stepResult = sqlite3_step(statement);
    std::cout << "ListAllVehicles sqlite3_step: " << stepResult;
    while (SQLITE_ROW == stepResult)
    {
        int id = sqlite3_column_int(statement, columnIndices["id"]);
// http://stackoverflow.com/questions/12052997/c-exception-occurred-in-script-basic-string-s-construct-null-not-valid
//        ustring type = sqlite3_column_text(statement, columnIndices["type"]);
//        ustring make = sqlite3_column_text(statement, columnIndices["make"]);
//        ustring model = sqlite3_column_text(statement, columnIndices["model"]);
        const unsigned char* type = sqlite3_column_text(statement, columnIndices["type"]);
        int typeSize = sqlite3_column_bytes(statement, columnIndices["type"]);

        const unsigned char* make = sqlite3_column_text(statement, columnIndices["make"]);
        int makeSize = sqlite3_column_bytes(statement, columnIndices["make"]);

        const unsigned char* model = sqlite3_column_text(statement, columnIndices["model"]);
        int modelSize = sqlite3_column_bytes(statement, columnIndices["model"]);

        int year = sqlite3_column_int(statement, columnIndices["year"]);
        int odometer = sqlite3_column_int(statement, columnIndices["odometer"]);

        std::unique_ptr<Vehicle> vehicle = std::unique_ptr<Vehicle>(new Vehicle(id));
        vehicle->GetType() = string(type, type + typeSize);
        vehicle->GetMake() = string(make, make + makeSize);
        vehicle->GetModel() = string(model, model + modelSize);
        vehicle->GetYear() = year;
        vehicle->GetOdometer() = odometer;

        vehicles->push_back(std::move(vehicle));

        stepResult = sqlite3_step(statement);
        std::cout << "ListAllVehicles sqlite3_step: " << stepResult;
    }

    sqlite3_reset(statement);
    sqlite3_clear_bindings(statement);

    return std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>(vehicles);
}

/// Search for vehicles matching the supplied vehicle properties
/// @param properties the vehicle properties to match
/// @param values the values of the vehicle properties to match
std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
Database::FindVehicles(Vehicle::Properties properties, const Vehicle& values)
{
	auto v = std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>();
	return v;
}

/// Get the vehicle with the supplied ID
/// @param vehicleId the vehicle ID to match
/// @return a vehicle with an ID > -1 if successful otherwise ID == -1
std::unique_ptr<Vehicle> Database::GetVehicle(int vehicleId)
{
	return std::unique_ptr<Vehicle>(new Vehicle);
}

/// Persists the vehicle values to storage
/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
/// return true if update/add otherwise false
bool Database::UpdateVehicle(Vehicle& vehicle)
{
	return false;
}

/// Persists the vehicle values to storage
/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
/// @param properties which properties to persist
/// @param userDefinedProperties which user defined properties to persist; ignored when Properties::All
/// return true if update/add otherwise false
bool Database::UpdateVehicle(Vehicle& vehicle,
	Vehicle::Properties properties,
	const std::vector<utf8string>& userDefinedProperties)
{
	return false;
}

/// Track a new type of maintenance
/// @param name new maintenance name
/// @return true if added otherwise false
bool Database::AddTypeOfMaintenance(const utf8string& name)
{
	return false;
}

/// Update the name of a maintenance type
/// @param name the current name
/// @param newName the new name of the maintenance type
/// @return true if updated/same otherwise false
bool Database::UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType)
{
	return false;
}

/// List available types of maintenance
/// @return the list of available maintenance types
std::unique_ptr<std::vector<utf8string>>
Database::ListAllTypesOfMaintenance()
{
	return std::unique_ptr<std::vector<utf8string>>(new std::vector<utf8string>());
}

/// Create and persist a new maintenance task
/// @return a vehicle with a persisted ID
std::unique_ptr<MaintenanceTask>
Database::CreateMaintenanceTask(int vehicleID)
{
	return std::unique_ptr<MaintenanceTask>(new MaintenanceTask);
}

/// Persists the maintenance task values to storage
/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
/// return true if update/add otherwise false
bool Database::UpdateMaintenanceTask(MaintenanceTask& task)
{
	return false;
}

/// Persists the maintenance task values to storage
/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
/// @param properties which properties to persist
/// return true if update/add otherwise false
bool Database::UpdateMaintenanceTask(MaintenanceTask& task,
	MaintenanceTask::Properties properties)
{
	return false;
}

/// Remove and no longer persist a maintenance task
/// @return true if task removed/not found otherwise false
bool Database::DeleteMaintenanceTask(MaintenanceTask& task)
{
	return false;
}