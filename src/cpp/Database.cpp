#include "Database.hpp"
#include "utils/Debug.h"
using namespace AMT;

Database::Database() :
    sqlite(nullptr),
	sqliteStatementMutex(),
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
    }

    return prepResult;
}

int Database::RetrieveVehiclePropsAndValues(Vehicle &vehicle,
						std::unordered_map<utf8string, utf8string>& propValMap)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* queryVehicleProperties = nullptr;
    static const std::string qtxtVehicleProperties(
		"SELECT Name FROM VehicleUserDefinedField WHERE ApplicableVehicleType=?");

    this->PrepareQuery(qtxtVehicleProperties, &queryVehicleProperties);

	static sqlite3_stmt* queryVehiclePropertyValue = nullptr;
    static const std::string qtxtVehiclePropertyValue(
		"SELECT Value FROM VehicleUserDefinedFieldValue WHERE VehicleID=? "
		"AND UserDefinedFieldName=?");

    this->PrepareQuery(qtxtVehiclePropertyValue, &queryVehiclePropertyValue);

    sqlite3_bind_text(queryVehicleProperties, 1,
					  vehicle.GetType().data(),
					  vehicle.GetType().size(),
					  NULL);

    int stepResult = sqlite3_step(queryVehicleProperties);

    while (SQLITE_ROW == stepResult)
    {
        const unsigned char* type =
			sqlite3_column_text(queryVehicleProperties, 0);
        int typeSize = sqlite3_column_bytes(queryVehicleProperties, 0);

        std::string vehicleProperty(type, type + typeSize);

        sqlite3_bind_int(queryVehiclePropertyValue, 1,
                         vehicle.GetID());

        sqlite3_bind_text(queryVehiclePropertyValue, 2,
                          vehicleProperty.data(),
                          vehicleProperty.size(),
                          NULL);

        stepResult = sqlite3_step(queryVehiclePropertyValue);

        if (SQLITE_ROW == stepResult)
        {
            const unsigned char* value =
				sqlite3_column_text(queryVehiclePropertyValue, 0);
            int valueSize = sqlite3_column_bytes(queryVehiclePropertyValue, 0);

            std::string vehiclePropertyValue(value, value + valueSize);

            propValMap[vehicleProperty] = vehiclePropertyValue;
        }

        sqlite3_reset(queryVehiclePropertyValue);
        sqlite3_clear_bindings(queryVehiclePropertyValue);

        stepResult = sqlite3_step(queryVehicleProperties);
    }

    sqlite3_reset(queryVehicleProperties);
    sqlite3_clear_bindings(queryVehicleProperties);

    return SQLITE_DONE;
}

bool Database::Setup(const char* databaseName, std::string& errorMessage)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static bool setupDone = false;
	if (setupDone)
		return true;

	errorMessage.clear();

	if (sqlite3_open_v2(databaseName,
                        &sqlite,
						SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
						NULL)
		!= SQLITE_OK)
	{
		errorMessage.append("Error opening the database ");
		errorMessage.append(databaseName);
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

    sqlite3_exec(this->sqlite,
		"CREATE TABLE IF NOT EXISTS VehicleUserDefinedField ("
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

	setupDone = (errorMessage.size() == 0);
	return setupDone;
}

bool Database::AddTypeOfVehicle(const utf8string& name)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText("INSERT INTO VehicleType (Name) VALUES (?)");
    this->PrepareQuery(queryText, &query);

    sqlite3_bind_text(query, 1, name.data(), name.size(), NULL);
	int stepResult = sqlite3_step(query);

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

	return (SQLITE_DONE == stepResult);
}

bool Database::UpdateTypesOfVehicles(const utf8string& name,
									 const utf8string& newName)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText("UPDATE VehicleType SET Name=? WHERE Name=?");
    this->PrepareQuery(queryText, &query);

	sqlite3_bind_text(query, 1, newName.data(), newName.size(), NULL);
    sqlite3_bind_text(query, 2, name.data(), name.size(), NULL);

    int stepResult = sqlite3_step(query);

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

	return (SQLITE_DONE == stepResult);
}

std::shared_ptr<std::vector<utf8string> > Database::ListAllTypesOfVehicles()
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText("SELECT * FROM VehicleType");
    this->PrepareQuery(queryText, &query);

    auto* allVehicleTypes = new std::vector<utf8string>();

    int stepResult = sqlite3_step(query);
	while (SQLITE_ROW == stepResult)
	{
        const unsigned char* nameBytes = sqlite3_column_text(query, 0);
        int nameSize = sqlite3_column_bytes(query, 0);

        std::string name(nameBytes, nameBytes + nameSize);
		allVehicleTypes->push_back(name);

        stepResult = sqlite3_step(query);
	}

    sqlite3_reset(query);

	return std::shared_ptr<std::vector<utf8string> >(allVehicleTypes);
}

std::shared_ptr<Vehicle> Database::CreateVehicle()
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText("INSERT INTO Vehicle (ID) VALUES (NULL)");
    this->PrepareQuery(queryText, &query);

    sqlite3_step(query);
    sqlite3_reset(query);

    sqlite3_int64 objectId = sqlite3_last_insert_rowid(this->sqlite);

    auto* vehicle = new Vehicle(objectId);
    return std::shared_ptr<Vehicle>(vehicle);
}

bool Database::DeleteVehicle(int vehicleID)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText("DELETE FROM Vehicle WHERE ID=?");
    this->PrepareQuery(queryText, &query);

	sqlite3_bind_int(query, 1, vehicleID);
    int stepResult = sqlite3_step(query);

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

    return (SQLITE_DONE == stepResult);
}

std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
	Database::ListAllVehicles()
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
    static const std::string queryText(
		"SELECT ID, Type, Make, Model, Year, Odometer FROM Vehicle");
    this->PrepareQuery(queryText, &query);

    auto* vehicles = new std::vector<std::shared_ptr<Vehicle> >;

    int stepResult = sqlite3_step(query);
    while (SQLITE_ROW == stepResult)
    {
        int id = sqlite3_column_int(query, 0);

        const unsigned char* type = sqlite3_column_text(query, 1);
        int typeSize = sqlite3_column_bytes(query, 1);

        const unsigned char* make = sqlite3_column_text(query, 2);
        int makeSize = sqlite3_column_bytes(query, 2);

        const unsigned char* model = sqlite3_column_text(query, 3);
        int modelSize = sqlite3_column_bytes(query, 3);

        int year = sqlite3_column_int(query, 4);
        int odometer = sqlite3_column_int(query, 5);

        std::shared_ptr<Vehicle> vehicle =
			std::shared_ptr<Vehicle>(new Vehicle(id));
        vehicle->GetType() = std::string(type, type + typeSize);
        vehicle->GetMake() = std::string(make, make + makeSize);
        vehicle->GetModel() = std::string(model, model + modelSize);
        vehicle->GetYear() = year;
        vehicle->GetOdometer() = odometer;

        vehicles->push_back(std::move(vehicle));

        stepResult = sqlite3_step(query);
    }

    sqlite3_reset(query);

    for (auto& vehicle : *vehicles)
    {
        std::unordered_map<utf8string, utf8string> propValMap;
        this->RetrieveVehiclePropsAndValues(*vehicle, propValMap);

        for (auto& kvp : propValMap)
        {
            vehicle->SetProperty(kvp.first, kvp.second);
        }
    }

    return std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >(vehicles);
}

std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
Database::FindVehicles(Vehicle::Properties properties, const Vehicle& values)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	auto v = std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >();
	return v;
}

std::shared_ptr<Vehicle> Database::GetVehicle(int vehicleID)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
    static const std::string queryText(
		"SELECT Type, Make, Model, Year, Odometer FROM Vehicle WHERE ID=?");
    this->PrepareQuery(queryText, &query);

	sqlite3_bind_int(query, 1, vehicleID);
	sqlite3_step(query);

    const unsigned char* type = sqlite3_column_text(query, 0);
    int typeSize = sqlite3_column_bytes(query, 0);

    const unsigned char* make = sqlite3_column_text(query, 1);
    int makeSize = sqlite3_column_bytes(query, 1);

    const unsigned char* model = sqlite3_column_text(query, 2);
    int modelSize = sqlite3_column_bytes(query, 2);

    int year = sqlite3_column_int(query, 3);
    int odometer = sqlite3_column_int(query, 4);

    std::shared_ptr<Vehicle> vehicle =
		std::shared_ptr<Vehicle>(new Vehicle(vehicleID));
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

bool Database::UpdateVehicle(Vehicle& vehicle)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

    if (vehicle.GetID() < 1)
    {
        auto newVehicle = this->CreateVehicle();
        vehicle.GetID() = std::move(newVehicle->GetID()); // might not need move
    }

	static sqlite3_stmt* queryUpdateVehicle = nullptr;
    static const std::string qtxtUpdateVehicle(
		"UPDATE Vehicle SET Type=?, Make=?, Model=?, Year=?, Odometer=? "
		"WHERE ID=?");
    this->PrepareQuery(qtxtUpdateVehicle, &queryUpdateVehicle);

    sqlite3_bind_text(queryUpdateVehicle, 1,
					  vehicle.GetType().data(),
					  vehicle.GetType().size(),
					  NULL);

    sqlite3_bind_text(queryUpdateVehicle, 2,
                      vehicle.GetMake().data(),
                      vehicle.GetMake().size(),
                      NULL);

    sqlite3_bind_text(queryUpdateVehicle, 3,
                      vehicle.GetModel().data(),
                      vehicle.GetModel().size(),
                      NULL);

    sqlite3_bind_int(queryUpdateVehicle, 4, vehicle.GetYear());
    sqlite3_bind_int(queryUpdateVehicle, 5, vehicle.GetOdometer());
    sqlite3_bind_int(queryUpdateVehicle, 6, vehicle.GetID());

    sqlite3_step(queryUpdateVehicle);

    sqlite3_reset(queryUpdateVehicle);
    sqlite3_clear_bindings(queryUpdateVehicle);

	//upsert
	//TODO add Transaction
	static sqlite3_stmt* queryInsertOrIgnorePropertyName = nullptr;
	static const std::string qtxt_Insert_or_Ignore_Property_Name(
		"INSERT OR IGNORE INTO VehicleUserDefinedField (Name, ApplicableVehicleType) "
		"VALUES (?, ?)");
	this->PrepareQuery(qtxt_Insert_or_Ignore_Property_Name, &queryInsertOrIgnorePropertyName);

	static sqlite3_stmt* queryInsertOrIgnorePropertyValue = nullptr;
	static const std::string qtxt_Insert_or_Ignore_Property_Value(
		"INSERT OR IGNORE INTO VehicleUserDefinedFieldValue "
		"(VehicleID, UserDefinedFieldName, Value) VALUES (?, ?, ?)");
	this->PrepareQuery(qtxt_Insert_or_Ignore_Property_Value, &queryInsertOrIgnorePropertyValue);

	static sqlite3_stmt* queryUpdatePropertyValue = nullptr;
	static const std::string qtxt_Update_Property_Value(
		"UPDATE VehicleUserDefinedFieldValue SET Value=? WHERE "
		"VehicleID=? AND UserDefinedFieldName=?");
	this->PrepareQuery(qtxt_Update_Property_Value, &queryUpdatePropertyValue);

	std::unordered_map<utf8string, utf8string> propValMap;
	this->RetrieveVehiclePropsAndValues(vehicle, propValMap);

    auto& vehicleProperties = vehicle.GetPropertyNames();
    for (auto& property : vehicleProperties)
	{
		auto& value = vehicle.GetProperty(property);

		sqlite3_bind_text(queryInsertOrIgnorePropertyName, 1,
						  property.data(),
						  property.size(),
						  NULL);

		sqlite3_bind_text(queryInsertOrIgnorePropertyName, 2,
						  vehicle.GetType().data(),
						  vehicle.GetType().size(),
						  NULL);

		sqlite3_step(queryInsertOrIgnorePropertyName);

		sqlite3_reset(queryInsertOrIgnorePropertyName);
		sqlite3_clear_bindings(queryInsertOrIgnorePropertyName);

		sqlite3_bind_int(queryInsertOrIgnorePropertyValue, 1, vehicle.GetID());

		sqlite3_bind_text(queryInsertOrIgnorePropertyValue, 2,
						  property.data(),
						  property.size(),
						  NULL);

		sqlite3_bind_text(queryInsertOrIgnorePropertyValue, 3,
						  value.data(),
						  value.size(),
						  NULL);

		sqlite3_step(queryInsertOrIgnorePropertyValue);

		sqlite3_reset(queryInsertOrIgnorePropertyValue);
		sqlite3_clear_bindings(queryInsertOrIgnorePropertyValue);
    }

	static sqlite3_stmt* queryDeletePropertyValue = nullptr;
	static const std::string qtxt_Delete_Property_Value(
		"DELETE FROM VehicleUserDefinedFieldValue "
		"WHERE VehicleID=? AND UserDefinedFieldName=?");
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

bool Database::UpdateVehicle(Vehicle& vehicle,
	Vehicle::Properties properties,
	const std::vector<utf8string>& userDefinedProperties)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	if (vehicle.GetID() < 1)
	{
		auto newVehicle = this->CreateVehicle();
		vehicle.GetID() = std::move(newVehicle->GetID()); // might not need move
	}

	std::string qtxtUpdateVehicle("UPDATE Vehicle SET ");

	int colCount = 1, typeIndex = 0, makeIndex = 0, modelIndex = 0, yearIndex = 0,
		odometerIndex = 0, IDIndex = 0, bindResult = 0;

	if ((int)properties & (int)Vehicle::Properties::Type)
	{
		typeIndex = colCount++;
		if (typeIndex > 1)
			qtxtUpdateVehicle.append(",");
		qtxtUpdateVehicle.append("Type=? ");
	}

	if ((int)properties & (int)Vehicle::Properties::Make)
	{
		makeIndex = colCount++;
		if (makeIndex > 1)
			qtxtUpdateVehicle.append(",");
		qtxtUpdateVehicle.append("Make=? ");
	}

	if ((int)properties & (int)Vehicle::Properties::Model)
	{
		modelIndex = colCount++;
		if (modelIndex > 1)
			qtxtUpdateVehicle.append(",");
		qtxtUpdateVehicle.append("Model=? ");
	}

	if ((int)properties & (int)Vehicle::Properties::Year)
	{
		yearIndex = colCount++;
		if (yearIndex > 1)
			qtxtUpdateVehicle.append(",");
		qtxtUpdateVehicle.append("Year=? ");
	}

	if ((int)properties & (int)Vehicle::Properties::Odometer)
	{
		odometerIndex = colCount++;
		if (odometerIndex > 1)
			qtxtUpdateVehicle.append(",");
		qtxtUpdateVehicle.append("Odometer=? ");
	}

	qtxtUpdateVehicle.append("WHERE ID=?");
	IDIndex = colCount++;

	sqlite3_stmt* queryUpdateVehicle = nullptr;
	this->PrepareQuery(qtxtUpdateVehicle, &queryUpdateVehicle);

	if ((int)properties & (int)Vehicle::Properties::Type)
	{
		sqlite3_bind_text(queryUpdateVehicle, typeIndex,
						  vehicle.GetType().data(),
						  vehicle.GetType().size(),
						  NULL);
	}

	if ((int)properties & (int)Vehicle::Properties::Make)
	{
		sqlite3_bind_text(queryUpdateVehicle, makeIndex,
						  vehicle.GetMake().data(),
						  vehicle.GetMake().size(),
						  NULL);
	}

	if ((int)properties & (int)Vehicle::Properties::Model)
	{
		sqlite3_bind_text(queryUpdateVehicle, modelIndex,
						  vehicle.GetModel().data(),
						  vehicle.GetModel().size(),
						  NULL);
	}

	if ((int)properties & (int)Vehicle::Properties::Year)
	{
		sqlite3_bind_int(queryUpdateVehicle, yearIndex, vehicle.GetYear());
	}

	if ((int)properties & (int)Vehicle::Properties::Odometer)
	{
		sqlite3_bind_int(queryUpdateVehicle, odometerIndex, vehicle.GetOdometer());
	}

	sqlite3_bind_int(queryUpdateVehicle, IDIndex, vehicle.GetID());

	int stepResult = sqlite3_step(queryUpdateVehicle);

	sqlite3_reset(queryUpdateVehicle);
	sqlite3_clear_bindings(queryUpdateVehicle);

	return (SQLITE_DONE == stepResult);
}

bool Database::AddTypeOfMaintenance(const utf8string& name)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText(
		"INSERT INTO MaintenanceType (Name) VALUES (?)");
    this->PrepareQuery(queryText, &query);

	sqlite3_bind_text(query, 1, name.data(), name.size(), NULL);
    int stepResult = sqlite3_step(query);

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

	return (SQLITE_DONE == stepResult);
}

bool Database::UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText(
		"UPDATE MaintenanceType SET Name=? WHERE Name=?");
    this->PrepareQuery(queryText, &query);

    sqlite3_bind_text(query, 1, newType.data(), newType.size(), NULL);
    sqlite3_bind_text(query, 2, type.data(), type.size(), NULL);

    int stepResult = sqlite3_step(query);

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

    return (SQLITE_DONE == stepResult);
}

std::shared_ptr<std::vector<utf8string> > Database::ListAllTypesOfMaintenance()
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string queryText("SELECT * FROM MaintenanceType");
	this->PrepareQuery(queryText, &query);

	auto* allTaskTypes = new std::vector<utf8string>();

	int stepResult = sqlite3_step(query);
	while (SQLITE_ROW == stepResult)
	{
		const unsigned char* nameBytes = sqlite3_column_text(query, 0);
		int nameSize = sqlite3_column_bytes(query, 0);

		std::string name(nameBytes, nameBytes + nameSize);
		allTaskTypes->push_back(name);

		stepResult = sqlite3_step(query);
	}

	sqlite3_reset(query);

	return std::shared_ptr<std::vector<utf8string> >(allTaskTypes);
}

std::shared_ptr<MaintenanceTask> Database::CreateMaintenanceTask(int vehicleID)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
    static const std::string queryText(
		"INSERT INTO Maintenance (VehicleID) VALUES (?)");
    this->PrepareQuery(queryText, &query);

	sqlite3_bind_int(query, 1, vehicleID);
    sqlite3_step(query);

    sqlite3_reset(query);
	sqlite3_clear_bindings(query);

    sqlite3_int64 objectID = sqlite3_last_insert_rowid(this->sqlite);

    return std::shared_ptr<MaintenanceTask>(new MaintenanceTask(objectID, vehicleID));
}

bool Database::UpdateMaintenanceTask(MaintenanceTask& task)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* queryInsertOrIgnoreMaintenanceType = nullptr;
	static const std::string qtxt_Insert_or_Ignore_Maintenance_Type(
		"INSERT OR IGNORE INTO MaintenanceType (Name) VALUES (?)");
	this->PrepareQuery(qtxt_Insert_or_Ignore_Maintenance_Type, &queryInsertOrIgnoreMaintenanceType);

	sqlite3_bind_text(queryInsertOrIgnoreMaintenanceType, 1,
					  task.GetType().data(),
					  task.GetType().size(),
					  NULL);

	int stepResult = sqlite3_step(queryInsertOrIgnoreMaintenanceType);
	bool succeeded = (SQLITE_DONE == stepResult);

	sqlite3_reset(queryInsertOrIgnoreMaintenanceType);
	sqlite3_clear_bindings(queryInsertOrIgnoreMaintenanceType);

	static sqlite3_stmt* query = nullptr;
	static const std::string qtxtUpdateTask(
		"UPDATE Maintenance SET VehicleID=?, Type=?, Date=? WHERE ID=?");
	this->PrepareQuery(qtxtUpdateTask, &query);

	if (task.GetID() < 1)
	{
		std::cout << "creating new task" << "\n";
		auto newTask = this->CreateMaintenanceTask(task.VehicleID());
		task.GetID() = std::move(newTask->GetID()); // might not need move
	}

	sqlite3_bind_int(query,  1, task.VehicleID());
	sqlite3_bind_text(query, 2, task.GetType().data(), task.GetType().size(), NULL);
	sqlite3_bind_int(query,  3, task.GetDate());
	sqlite3_bind_int(query,  4, task.GetID());

	stepResult = sqlite3_step(query);
	succeeded &= (SQLITE_DONE == stepResult);

	sqlite3_reset(query);
	sqlite3_clear_bindings(query);

	return succeeded;
}

bool Database::UpdateMaintenanceTask(MaintenanceTask& task,
									 MaintenanceTask::Properties properties)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);
	return false;
}

bool Database::DeleteMaintenanceTask(int taskID)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
    static const std::string queryText("DELETE FROM Maintenance WHERE ID=?");
    this->PrepareQuery(queryText, &query);

    sqlite3_bind_int(query, 1, taskID);
    int stepResult = sqlite3_step(query);

    sqlite3_reset(query);
    sqlite3_clear_bindings(query);

    return (SQLITE_DONE == stepResult);
}

std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
	Database::ListVehicleMaintenanceHistory(int vehicleID)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);

	static sqlite3_stmt* query = nullptr;
	static const std::string qtxtMaintenanceHistory(
		"SELECT ID, Type, Date FROM Maintenance WHERE VehicleID=?");
	this->PrepareQuery(qtxtMaintenanceHistory, &query);

	sqlite3_bind_int(query, 1, vehicleID);

	auto* tasks = new std::vector<std::shared_ptr<MaintenanceTask> >();

	int stepResult = sqlite3_step(query);
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
	}

	sqlite3_reset(query);
	sqlite3_clear_bindings(query);

	return std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >(tasks);
}

std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
Database::ListVehicleMaintenanceHistory(int vehicleID, int startDate, int endDate)
{
	std::lock_guard<std::mutex>(this->sqliteStatementMutex);
	return nullptr;
}
