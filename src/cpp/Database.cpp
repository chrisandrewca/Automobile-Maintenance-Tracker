#include "Database.hpp"
#include "utils/Debug.h"
using namespace AMT;

Database::Database() :
    sqlite(nullptr),
    preparedStatements()
{
}

Database::~Database()
{
    for (auto& statement : preparedStatements)
    {
        if (statement)
        {
            sqlite3_finalize(statement);
        }
    }

    if (this->sqlite)
        sqlite3_close_v2(this->sqlite);
}

bool Database::Open(const std::string& name, std::string& errorMessage)
{
	return this->Setup(name.data(), errorMessage);
}

int Database::PrepareQuery(const std::string& queryText, sqlite3_stmt** preparedQuery)
{
    int prepResult = -1;
    if (!*preparedQuery)
    {
        prepResult = sqlite3_prepare_v2(this->sqlite,
                                        queryText.data(),
                                        queryText.size(),
                                        preparedQuery,
                                        NULL);
        if (SQLITE_OK == prepResult)
            preparedStatements.push_back(*preparedQuery);

        std::cout << "sqlite3_prepare_v2: " << prepResult << "\n";
    }

    return prepResult;
}

int Database::RetrieveVehiclePropsAndValues(Vehicle &vehicle, std::unordered_map<utf8string, utf8string> &propValMap)
{
    //std::cout << "compiling qtxtVehicleProperties" << "\n";
    static const std::string qtxtVehicleProperties("SELECT Name FROM VehicleUserDefinedField WHERE ApplicableVehicleType=?");
    static sqlite3_stmt* queryVehicleProperties = nullptr;
    this->PrepareQuery(qtxtVehicleProperties, &queryVehicleProperties);

    //std::cout << "compiling qtxtVehiclePropertyValue" << "\n";
    static const std::string qtxtVehiclePropertyValue("SELECT Value FROM VehicleUserDefinedFieldValue WHERE VehicleID=? AND UserDefinedFieldName=?");
    static sqlite3_stmt* queryVehiclePropertyValue = nullptr;
    this->PrepareQuery(qtxtVehiclePropertyValue, &queryVehiclePropertyValue);

    int bindResult = sqlite3_bind_text(queryVehicleProperties, 1,
                                   vehicle.GetType().data(),
                                   vehicle.GetType().size(),
                                   NULL);
    std::cout << "ListAllVehicles sqlite3_bind_text: " << bindResult << "\n";

    int stepResult = sqlite3_step(queryVehicleProperties);
    std::cout << "ListAllVehicles sqlite3_step: " << stepResult << "\n";

    while (SQLITE_ROW == stepResult)
    {
        const unsigned char* type = sqlite3_column_text(queryVehicleProperties,
                                                        0);
        int typeSize = sqlite3_column_bytes(queryVehicleProperties, 0);
        std::string vehicleProperty(type, type + typeSize);
        std::cout << "got vehicle property " << vehicleProperty << "\n";

        bindResult = sqlite3_bind_int(queryVehiclePropertyValue, 1,
                                      vehicle.GetID());
        std::cout << "ListAllVehicles sqlite3_bind_int: " << bindResult << "\n";

        bindResult = sqlite3_bind_text(queryVehiclePropertyValue, 2,
                                       vehicleProperty.data(),
                                       vehicleProperty.size(),
                                       NULL);
        std::cout << "ListAllVehicles sqlite3_bind_text: " << bindResult << "\n";

        stepResult = sqlite3_step(queryVehiclePropertyValue);
        std::cout << "ListAllVehicles sqlite3_step: " << stepResult << "\n";

        if (SQLITE_ROW == stepResult)
        {
            const unsigned char* value = sqlite3_column_text(queryVehiclePropertyValue, 0);
            int valueSize = sqlite3_column_bytes(queryVehiclePropertyValue, 0);
            std::string vehiclePropertyValue(value, value + valueSize);
            std::cout << "got vehicle prop value " << vehiclePropertyValue << "\n";

            propValMap[vehicleProperty] = vehiclePropertyValue;
        }

        sqlite3_reset(queryVehiclePropertyValue);
        sqlite3_clear_bindings(queryVehiclePropertyValue);

        stepResult = sqlite3_step(queryVehicleProperties);
        std::cout << "ListAllVehicles sqlite3_step: " << stepResult << "\n";
    }

    sqlite3_reset(queryVehicleProperties);
    sqlite3_clear_bindings(queryVehicleProperties);

    // TODO error handling / logging
    return SQLITE_DONE;
}

bool Database::Setup(const char* databaseName, std::string& errorMessage)
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
			"VehicleID INTEGER NOT NULL REFERENCES Vehicle (ID),"
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

	return (errorMessage.size() == 0);
}

bool
Database::AddTypeOfVehicle(const utf8string& name)
{
    // "register" mechanism
    static const char* queryText = "INSERT INTO VehicleType (Name) VALUES (?)";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

	bool succeeded = false;
	/// !!! important
	// need mutex on SQLitePreparedStatementPtr
	// for functions which Bind + Step
		// API, Datastore errors
			// std::timed_mutex - succeed || timeout
			// support C++ library & http nicely

	/// !!! TODO ERROR CHECKING LOGGING
    int bindResult = sqlite3_bind_text(query, 1, name.data(), name.size(), NULL);
	std::cout << "AddTypeOfVehicle sqlite3_bind_text: " << bindResult << "\n";

	/// !!! TODO ERROR CHECKING LOGGING
    int stepResult = sqlite3_step(query);
	std::cout << "AddTypeOfVehicle sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

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

    static const char* queryText = "UPDATE VehicleType SET Name=? WHERE Name=?";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

	bool succeeded = false;

	/// !!! TODO ERROR CHECKING LOGGING
    int bindResult = sqlite3_bind_text(query, 1, newName.data(), newName.size(), NULL);
	std::cout << "UpdateTypesOfVehicles sqlite3_bind_text: " << bindResult << "\n";

    bindResult = sqlite3_bind_text(query, 2, name.data(), name.size(), NULL);
	std::cout << "UpdateTypesOfVehicles sqlite3_bind_text: " << bindResult << "\n";

	/// !!! TODO ERROR CHECKING LOGGING
    int stepResult = sqlite3_step(query);
	std::cout << "UpdateTypesOfVehicles sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

	return succeeded;
}

std::shared_ptr<std::vector<utf8string> >
Database::ListAllTypesOfVehicles()
{
	/// !!! TODO ERROR CHECKING LOGGING
	/// !!! important
	// need mutex on SQLitePreparedStatementPtr
	// for functions which Bind, Step
	// API, Datastore errors
	// std::timed_mutex - succeed || timeout
	// support C++ library & http nicely

    static const char* queryText = "SELECT * FROM VehicleType";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

    auto* allVehicleTypes = new std::vector<utf8string>();

    int stepResult = sqlite3_step(query);
	std::cout << "ListAllTypesOfVehicles sqlite3_step: " << stepResult << "\n";

	while (SQLITE_ROW == stepResult)
	{
        const unsigned char* nameBytes = sqlite3_column_text(query, 0);
        int nameSize = sqlite3_column_bytes(query, 0);

        std::string name(nameBytes, nameBytes + nameSize);
		allVehicleTypes->push_back(name);

        stepResult = sqlite3_step(query);
		std::cout << "ListAllTypesOfVehicles sqlite3_step: " << stepResult << "\n";
	}

    sqlite3_reset(query);

	return std::shared_ptr<std::vector<utf8string> >(allVehicleTypes);
}

std::shared_ptr<Vehicle>
Database::CreateVehicle()
{
    /// !!! TODO ERROR CHECKING LOGGING
    /// !!! important
    // need mutex on SQLitePreparedStatementPtr
    // for functions which Bind, Step
    // API, Datastore errors
    // std::timed_mutex - succeed || timeout
    // support C++ library & http nicely

    static const char* queryText = "INSERT INTO Vehicle (ID) VALUES (NULL)";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

    int stepResult = sqlite3_step(query);
    std::cout << "CreateVehicle sqlite3_step: " << stepResult << "\n";
    sqlite3_reset(query);

    sqlite3_int64 objectId = sqlite3_last_insert_rowid(this->sqlite);

    auto* vehicle = new Vehicle(objectId);
    return std::shared_ptr<Vehicle>(vehicle);
}

bool
Database::DeleteVehicle(int vehicleID)
{
    /// !!! TODO ERROR CHECKING LOGGING
    /// !!! important
    // need mutex on SQLitePreparedStatementPtr
    // for functions which Bind, Step
    // API, Datastore errors
    // std::timed_mutex - succeed || timeout
    // support C++ library & http nicely

    static const char* queryText = "DELETE FROM Vehicle WHERE ID=?";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

    int bindResult = sqlite3_bind_int(query, 1, vehicleID);
    std::cout << "DeleteVehicle sqlite3_bind_int: " << bindResult;

    int stepResult = sqlite3_step(query);
    std::cout << "CreateVehicle sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

    return (SQLITE_DONE == stepResult);
}

/// List all available vehicles
/// @return the list of all available vehicles
std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
Database::ListAllVehicles()
{
    /// !!! TODO ERROR CHECKING LOGGING
    /// !!! important
    // need mutex on SQLitePreparedStatementPtr
    // for functions which Bind, Step
    // API, Datastore errors
    // std::timed_mutex - succeed || timeout
    // support C++ library & http nicely

    std::cout << "inside ListAllVehicles" << "\n";

    // TODO use column names to show positions rather than *
    static const std::string queryText("SELECT ID, Type, Make, Model, Year, Odometer FROM Vehicle");
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

    auto* vehicles = new std::vector<std::shared_ptr<Vehicle> >;

    int stepResult = sqlite3_step(query);
    std::cout << "ListAllVehicles sqlite3_step: " << stepResult << "\n";
    while (SQLITE_ROW == stepResult)
    {
        int id = sqlite3_column_int(query, 0);
// http://stackoverflow.com/questions/12052997/c-exception-occurred-in-script-basic-string-s-construct-null-not-valid
//        ustring type = sqlite3_column_text(statement, columnIndices["type"]);
        const unsigned char* type = sqlite3_column_text(query, 1);
        int typeSize = sqlite3_column_bytes(query, 1);

        const unsigned char* make = sqlite3_column_text(query, 2);
        int makeSize = sqlite3_column_bytes(query, 2);

        const unsigned char* model = sqlite3_column_text(query, 3);
        int modelSize = sqlite3_column_bytes(query, 3);

        int year = sqlite3_column_int(query, 4);
        int odometer = sqlite3_column_int(query, 5);

        std::shared_ptr<Vehicle> vehicle = std::shared_ptr<Vehicle>(new Vehicle(id));
        vehicle->GetType() = std::string(type, type + typeSize);
        vehicle->GetMake() = std::string(make, make + makeSize);
        vehicle->GetModel() = std::string(model, model + modelSize);
        vehicle->GetYear() = year;
        vehicle->GetOdometer() = odometer;

        vehicles->push_back(std::move(vehicle));

        stepResult = sqlite3_step(query);
        std::cout << "ListAllVehicles sqlite3_step: " << stepResult << "\n";
    }

    sqlite3_reset(query);

    for (auto& vehicle : *vehicles)
    {
        std::unordered_map<utf8string, utf8string> propValMap;
        this->RetrieveVehiclePropsAndValues(*vehicle, propValMap);
        for (auto& kvp : propValMap)
        {
            std::cout << "prop " << kvp.first << "\n";
            std::cout << "val " << kvp.second << "\n";
            vehicle->SetProperty(kvp.first, kvp.second);
        }
    }

    return std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >(vehicles);
}

/// Search for vehicles matching the supplied vehicle properties
/// @param properties the vehicle properties to match
/// @param values the values of the vehicle properties to match
std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
Database::FindVehicles(Vehicle::Properties properties, const Vehicle& values)
{
	auto v = std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >();
	return v;
}

/// Get the vehicle with the supplied ID
/// @param vehicleId the vehicle ID to match
/// @return a vehicle with an ID > -1 if successful otherwise ID == -1
std::shared_ptr<Vehicle> Database::GetVehicle(int vehicleID)
{
    static const std::string queryText("SELECT Type, Make, Model, Year, Odometer FROM Vehicle "
									   "WHERE ID=?");
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

	int bindResult = sqlite3_bind_int(query, 1, vehicleID);
	std::cout << "GetVehicle sqlite3_bind_int: " << bindResult;

	int stepResult = sqlite3_step(query);
    std::cout << "GetVehicle sqlite3_step: " << stepResult << "\n";

    const unsigned char* type = sqlite3_column_text(query, 0);
    int typeSize = sqlite3_column_bytes(query, 0);

    const unsigned char* make = sqlite3_column_text(query, 1);
    int makeSize = sqlite3_column_bytes(query, 1);

    const unsigned char* model = sqlite3_column_text(query, 2);
    int modelSize = sqlite3_column_bytes(query, 2);

    int year = sqlite3_column_int(query, 3);
    int odometer = sqlite3_column_int(query, 4);

    std::shared_ptr<Vehicle> vehicle = std::shared_ptr<Vehicle>(new Vehicle(vehicleID));
    vehicle->GetType() = std::string(type, type + typeSize);
    vehicle->GetMake() = std::string(make, make + makeSize);
    vehicle->GetModel() = std::string(model, model + modelSize);
    vehicle->GetYear() = year;
    vehicle->GetOdometer() = odometer;

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

	std::unordered_map<utf8string, utf8string> propValMap;
	this->RetrieveVehiclePropsAndValues(*vehicle, propValMap);
	for (auto& kvp : propValMap)
	{
		vehicle->SetProperty(kvp.first, kvp.second);
	}

	return vehicle;
}

/// Persists the vehicle values to storage
/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
/// return true if update/add otherwise false
bool Database::UpdateVehicle(Vehicle& vehicle)
{
    std::cout << "inside UpdateVehicle" << "\n";

    if (vehicle.GetID() < 1)
    {
        std::cout << "creating new vehicle" << "\n";
        auto newVehicle = this->CreateVehicle();
        vehicle.GetID() = std::move(newVehicle->GetID()); // might not need move
    }

    static const std::string qtxtUpdateVehicle(
                                "UPDATE Vehicle SET "
                                "Type=?, Make=?, Model=?, Year=?, Odometer=? "
                                "WHERE ID=?"); // if id doesnt exist return helpful error
    static sqlite3_stmt* queryUpdateVehicle = nullptr;
    this->PrepareQuery(qtxtUpdateVehicle, &queryUpdateVehicle);

    int bindResult = sqlite3_bind_text(queryUpdateVehicle, 1,
                                       vehicle.GetType().data(),
                                       vehicle.GetType().size(),
                                       NULL);
    std::cout << "UpdateVehicle sqlite3_bind_text: " << bindResult << "\n";

    bindResult = sqlite3_bind_text(queryUpdateVehicle, 2,
                                    vehicle.GetMake().data(),
                                    vehicle.GetMake().size(),
                                    NULL);
    std::cout << "UpdateVehicle sqlite3_bind_text: " << bindResult << "\n";

    bindResult = sqlite3_bind_text(queryUpdateVehicle, 3,
                                    vehicle.GetModel().data(),
                                    vehicle.GetModel().size(),
                                    NULL);
    std::cout << "UpdateVehicle sqlite3_bind_text: " << bindResult << "\n";

    std::cout << "year " << vehicle.GetYear() << "\n";
    bindResult = sqlite3_bind_int(queryUpdateVehicle, 4, vehicle.GetYear());
    std::cout << "UpdateVehicle sqlite3_bind_int: " << bindResult << "\n";

    std::cout << "odometer " << vehicle.GetOdometer() << "\n";
    bindResult = sqlite3_bind_int(queryUpdateVehicle, 5, vehicle.GetOdometer());
    std::cout << "UpdateVehicle sqlite3_bind_int: " << bindResult << "\n";

    bindResult = sqlite3_bind_int(queryUpdateVehicle, 6, vehicle.GetID());
    std::cout << "UpdateVehicle sqlite3_bind_int: " << bindResult << "\n";

    int stepResult = sqlite3_step(queryUpdateVehicle);
    std::cout << "UpdateVehicle sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(queryUpdateVehicle);
    sqlite3_clear_bindings(queryUpdateVehicle);

	//upsert
	//TODO add Transaction
	static const std::string qtxt_Insert_or_Ignore_Property_Name(
		"INSERT OR IGNORE INTO VehicleUserDefinedField (Name, ApplicableVehicleType) "
		"VALUES (?, ?)");
	static sqlite3_stmt* queryInsertOrIgnorePropertyName = nullptr;
	this->PrepareQuery(qtxt_Insert_or_Ignore_Property_Name, &queryInsertOrIgnorePropertyName);

	static const std::string qtxt_Insert_or_Ignore_Property_Value(
		"INSERT OR IGNORE INTO VehicleUserDefinedFieldValue "
		"(VehicleID, UserDefinedFieldName, Value) VALUES (?, ?, ?)");
	static sqlite3_stmt* queryInsertOrIgnorePropertyValue = nullptr;
	this->PrepareQuery(qtxt_Insert_or_Ignore_Property_Value, &queryInsertOrIgnorePropertyValue);

	static const std::string qtxt_Update_Property_Value(
		"UPDATE VehicleUserDefinedFieldValue SET Value=? WHERE "
		"VehicleID=? AND UserDefinedFieldName=?");
	static sqlite3_stmt* queryUpdatePropertyValue = nullptr;
	this->PrepareQuery(qtxt_Update_Property_Value, &queryUpdatePropertyValue);

	std::unordered_map<utf8string, utf8string> propValMap;
	this->RetrieveVehiclePropsAndValues(vehicle, propValMap);

    auto& vehicleProperties = vehicle.GetPropertyNames();
    for (auto& property : vehicleProperties)
	{
		auto& value = vehicle.GetProperty(property);

		int bindResult = sqlite3_bind_text(queryInsertOrIgnorePropertyName, 1,
			property.data(),
			property.size(),
			NULL);
		std::cout << "UpdateVehicle bind_text: " << bindResult;

		bindResult = sqlite3_bind_text(queryInsertOrIgnorePropertyName, 2,
			vehicle.GetType().data(),
			vehicle.GetType().size(),
			NULL);
		std::cout << "UpdateVehicle bind_text: " << bindResult;

		int stepResult = sqlite3_step(queryInsertOrIgnorePropertyName);
		std::cout << "UpdateVehicle step: " << stepResult;

		sqlite3_reset(queryInsertOrIgnorePropertyName);
		sqlite3_clear_bindings(queryInsertOrIgnorePropertyName);

		bindResult = sqlite3_bind_int(queryInsertOrIgnorePropertyValue, 1, vehicle.GetID());
		std::cout << "UpdateVehicle bind_int: " << bindResult;

		bindResult = sqlite3_bind_text(queryInsertOrIgnorePropertyValue, 2,
			property.data(),
			property.size(),
			NULL);
		std::cout << "UpdateVehicle bind_text: " << bindResult;

		bindResult = sqlite3_bind_text(queryInsertOrIgnorePropertyValue, 3,
			value.data(),
			value.size(),
			NULL);
		std::cout << "UpdateVehicle bind_text: " << bindResult;

		stepResult = sqlite3_step(queryInsertOrIgnorePropertyValue);
		std::cout << "UpdateVehicle step: " << stepResult;

		sqlite3_reset(queryInsertOrIgnorePropertyValue);
		sqlite3_clear_bindings(queryInsertOrIgnorePropertyValue);
    }

	static const std::string qtxt_Delete_Property_Value(
		"DELETE FROM VehicleUserDefinedFieldValue "
		"WHERE VehicleID=? AND UserDefinedFieldName=?");
	static sqlite3_stmt* queryDeletePropertyValue = nullptr;
	this->PrepareQuery(qtxt_Delete_Property_Value, &queryDeletePropertyValue);

	// TODO: add interface to AMT::Vehicle for removing a property
	// remove property from db
	// foreach dbPropValue
		// if dbPropValue not in vPropValues
			// delete dbPropValue value 
			// TODO: Add AMT::APIBase to delete PropertyName->VehicleType association
					// as it is an expensive operation

    return true;
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
    // deleting property names?
	return false;
}

/// Track a new type of maintenance
/// @param name new maintenance name
/// @return true if added otherwise false
bool Database::AddTypeOfMaintenance(const utf8string& name)
{
    static const char* queryText = "INSERT INTO MaintenanceType (Name) VALUES (?)";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

    bool succeeded = false;

    int bindResult = sqlite3_bind_text(query, 1, name.data(), name.size(), NULL);
    std::cout << "AddTypeOfMaintenance sqlite3_bind_text: " << bindResult << "\n";

    int stepResult = sqlite3_step(query);
    std::cout << "AddTypeOfMaintenance sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

    return succeeded;
}

/// Update the name of a maintenance type
/// @param name the current name
/// @param newName the new name of the maintenance type
/// @return true if updated/same otherwise false
bool Database::UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType)
{
    static const char* queryText = "UPDATE MaintenanceType SET Name=? WHERE Name=?";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

    bool succeeded = false;

    int bindResult = sqlite3_bind_text(query, 1, newType.data(), newType.size(), NULL);
    std::cout << "UpdateTypesOfVehicles sqlite3_bind_text: " << bindResult << "\n";

    bindResult = sqlite3_bind_text(query, 2, type.data(), type.size(), NULL);
    std::cout << "UpdateTypesOfVehicles sqlite3_bind_text: " << bindResult << "\n";

    int stepResult = sqlite3_step(query);
    std::cout << "UpdateTypesOfVehicles sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

    return succeeded;
}

/// List available types of maintenance
/// @return the list of available maintenance types
std::shared_ptr<std::vector<utf8string> >
Database::ListAllTypesOfMaintenance()
{
    //std::cout << "inside ListAllTypesOfMaintenance" << "\n";

    //// TODO use column names to show positions rather than *
    //static const std::string queryText("SELECT ID, VehicleID, Type, Date FROM Maintenance");
    //static sqlite3_stmt* query = nullptr;
    //this->PrepareQuery(queryText, &query);

    //auto* maintenanceTasks = new std::vector<std::shared_ptr<MaintenanceTask> >;

    //int stepResult = sqlite3_step(query);
    //std::cout << "ListAllTypesOfMaintenance sqlite3_step: " << stepResult << "\n";

    //while (SQLITE_ROW == stepResult)
    //{
    //    int id = sqlite3_column_int(query, 0);
    //    int vehicleId = sqlite3_column_int(query, 1);

    //    const unsigned char* type = sqlite3_column_text(query, 2);
    //    int typeSize = sqlite3_column_bytes(query, 2);

    //    int date = sqlite3_column_int(query, 3);

    //    std::shared_ptr<MaintenanceTask> task = std::shared_ptr<MaintenanceTask>(new MaintenanceTask(id));
    //    task->VehicleID() = vehicleId;
    //    task->GetType() = std::string(type, type + typeSize);
    //    task->GetDate() = date;

    //    maintenanceTasks->push_back(std::move(task));

    //    stepResult = sqlite3_step(query);
    //    std::cout << "ListAllTypesOfMaintenance sqlite3_step: " << stepResult << "\n";
    //}

    //sqlite3_reset(query);

	return std::shared_ptr<std::vector<utf8string> >(new std::vector<utf8string>());
}

/// Create and persist a new maintenance task
/// @return a vehicle with a persisted ID
std::shared_ptr<MaintenanceTask>
Database::CreateMaintenanceTask(int vehicleID)
{
    // TODO update DB to have constraint on vehicleID existing
    static const char* queryText = "INSERT INTO Maintenance (VehicleID) VALUES (?)";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

	int bindResult = sqlite3_bind_int(query, 1, vehicleID);
	std::cout << "CreateMaintenanceTask sqlite3_bind_int: " << bindResult << "\n";

    int stepResult = sqlite3_step(query);
    std::cout << "CreateMaintenanceTask sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(query);
	sqlite3_clear_bindings(query);

    sqlite3_int64 objectID = sqlite3_last_insert_rowid(this->sqlite);

    return std::shared_ptr<MaintenanceTask>(new MaintenanceTask(objectID, vehicleID));
}

/// Persists the maintenance task values to storage
/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
/// return true if update/add otherwise false
bool Database::UpdateMaintenanceTask(MaintenanceTask& task)
{
	static const std::string qtxt_Insert_or_Ignore_Maintenance_Type(
		"INSERT OR IGNORE INTO MaintenanceType (Name) VALUES (?)");
	static sqlite3_stmt* queryInsertOrIgnoreMaintenanceType = nullptr;
	this->PrepareQuery(qtxt_Insert_or_Ignore_Maintenance_Type, &queryInsertOrIgnoreMaintenanceType);

	int bindResult = sqlite3_bind_text(queryInsertOrIgnoreMaintenanceType, 1,
		task.GetType().data(),
		task.GetType().size(),
		NULL);
	std::cout << "UpdateMaintenanceTask sqlite3_bind_int: " << bindResult << "\n";

	int stepResult = sqlite3_step(queryInsertOrIgnoreMaintenanceType);
	std::cout << "UpdateMaintenanceTask sqlite3_step: " << stepResult << "\n";

	static const char* qtxtUpdateTask = "UPDATE Maintenance SET VehicleID=?, Type=?, Date=? "
										"WHERE ID=?";
	static sqlite3_stmt* query = nullptr;
	this->PrepareQuery(qtxtUpdateTask, &query);

	if (task.GetID() < 1)
	{
		std::cout << "creating new task" << "\n";
		auto newTask = this->CreateMaintenanceTask(task.VehicleID());
		task.GetID() = std::move(newTask->GetID()); // might not need move
	}

	bindResult = sqlite3_bind_int(query, 1, task.VehicleID());
	std::cout << "UpdateMaintenanceTask sqlite3_bind_int: " << bindResult << "\n";

	bindResult = sqlite3_bind_text(query, 2, task.GetType().data(), task.GetType().size(), NULL);
	std::cout << "UpdateMaintenanceTask sqlite3_bind_int: " << bindResult << "\n";

	bindResult = sqlite3_bind_int(query, 3, task.GetDate());
	std::cout << "UpdateMaintenanceTask sqlite3_bind_int: " << bindResult << "\n";

	bindResult = sqlite3_bind_int(query, 4, task.GetID());
	std::cout << "UpdateMaintenanceTask sqlite3_bind_int: " << bindResult << "\n";

	stepResult = sqlite3_step(query);
	std::cout << "UpdateMaintenanceTask sqlite3_step: " << stepResult << "\n";

	sqlite3_reset(query);
	sqlite3_clear_bindings(query);

	return true;
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
bool Database::DeleteMaintenanceTask(int taskID)
{
    static const char* queryText = "DELETE FROM Maintenance WHERE ID=?";
    static sqlite3_stmt* query = nullptr;
    this->PrepareQuery(queryText, &query);

    int bindResult = sqlite3_bind_int(query, 1, taskID);
	std::cout << "DeleteMaintenanceTask sqlite3_bind_int: " << bindResult << "\n";

    int stepResult = sqlite3_step(query);
    std::cout << "DeleteMaintenanceTask sqlite3_step: " << stepResult << "\n";

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

    return (SQLITE_DONE == stepResult);
}

/// List the entire maintenance history of the vehicle
/// @param vehicleID the vehicle's ID
/// @return a list of maintenance tasks associated with the vehicle
std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
Database::ListVehicleMaintenanceHistory(int vehicleID)
{
	std::cout << "Inside ListVehicleMaintenanceHistory" << "\n";
	static const char* qtxtMaintenanceHistory = "SELECT ID, Type, Date FROM Maintenance "
												"WHERE VehicleID=?";
	static sqlite3_stmt* query = nullptr;
	this->PrepareQuery(qtxtMaintenanceHistory, &query);

	int bindResult = sqlite3_bind_int(query, 1, vehicleID);
	std::cout << "ListVehicleMaintenanceHistory sqlite3_bind_int: " << bindResult << "\n";

	int stepResult = sqlite3_step(query);
    std::cout << "ListVehicleMaintenanceHistory sqlite3_step: " << stepResult << "\n";

	auto* tasks = new std::vector<std::shared_ptr<MaintenanceTask> >();
	while (SQLITE_ROW == stepResult)
	{
		int objectID = sqlite3_column_int(query, 0);
		
		const unsigned char* type = sqlite3_column_text(query, 1);
		int typeSize = sqlite3_column_bytes(query, 1);

		int date = sqlite3_column_int(query, 2);

		auto* task = new MaintenanceTask(objectID, vehicleID);
		task->GetType() = std::string(type, type + typeSize);
		task->GetDate() = date;

		tasks->push_back(std::move(std::shared_ptr<MaintenanceTask>(task)));

		stepResult = sqlite3_step(query);
		std::cout << "ListVehicleMaintenanceHistory sqlite3_step: " << stepResult << "\n";
	}

	sqlite3_reset(query);
	sqlite3_clear_bindings(query);

	return std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >(tasks);
}

/// List the entire maintenance history of the vehicle
/// @param vehicleID the vehicle's ID
/// @param startDate the earliest maintenance history date inclusive
/// @param endDate the latest maintenance history date inclusive
/// @return a list of maintenance tasks associated with the vehicle
std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
Database::ListVehicleMaintenanceHistory(int vehicleID, int startDate, int endDate)
{
	return nullptr;
}
