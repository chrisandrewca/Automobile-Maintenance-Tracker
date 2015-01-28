#include "AutomobileMaintenanceTracker.hpp"
#include "Database.hpp"
using namespace AMT;

AMTAPI::AMTAPI(AMTAPI::DataStoreOption dataStore) :
	amt()
{
	switch (dataStore)
	{
	case DataStoreOption::Database:
		std::string errMsg;
		auto* database = new Database;
		database->Open("auto_maintenance_tracker", errMsg);
		amt.reset(database);
		break;
	}
}

AMTAPI::~AMTAPI(){}

bool
AMTAPI::AddTypeOfVehicle(const utf8string& name)
{
	return this->amt->AddTypeOfVehicle(name);
}

bool
AMTAPI::UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName)
{
	return this->amt->UpdateTypesOfVehicles(name, newName);
}

std::shared_ptr<std::vector<utf8string>>
AMTAPI::ListAllTypesOfVehicles()
{
    return this->amt->ListAllTypesOfVehicles();
}

std::shared_ptr<Vehicle>
AMTAPI::CreateVehicle()
{
    return this->amt->CreateVehicle();
}

bool
AMTAPI::DeleteVehicle(int vehicleID)
{
    return this->amt->DeleteVehicle(vehicleID);
}

/// List all available vehicles
/// @return the list of all available vehicles
std::shared_ptr<std::vector<std::shared_ptr<Vehicle>>>
AMTAPI::ListAllVehicles()
{
    return this->amt->ListAllVehicles();
}

/// Search for vehicles matching the supplied vehicle properties
/// @param properties the vehicle properties to match
/// @param values the values of the vehicle properties to match
std::shared_ptr<std::vector<std::shared_ptr<Vehicle>>>
AMTAPI::FindVehicles(Vehicle::Properties properties, const Vehicle& values)
{
    return this->amt->FindVehicles(properties, values);
}

/// Get the vehicle with the supplied ID
/// @param vehicleId the vehicle ID to match
/// @return a vehicle with an ID > -1 if successful otherwise ID == -1
std::shared_ptr<Vehicle> AMTAPI::GetVehicle(int vehicleID)
{
    return this->amt->GetVehicle(vehicleID);
}

/// Persists the vehicle values to storage
/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
/// return true if update/add otherwise false
bool AMTAPI::UpdateVehicle(Vehicle& vehicle)
{
    return this->amt->UpdateVehicle(vehicle);
}

/// Persists the vehicle values to storage
/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
/// @param properties which properties to persist
/// @param userDefinedProperties which user defined properties to persist; ignored when Properties::All
/// return true if update/add otherwise false
bool AMTAPI::UpdateVehicle(Vehicle& vehicle,
	Vehicle::Properties properties,
	const std::vector<utf8string>& userDefinedProperties)
{
    return this->amt->UpdateVehicle(vehicle, properties, userDefinedProperties);
}

/// Track a new type of maintenance
/// @param name new maintenance name
/// @return true if added otherwise false
bool AMTAPI::AddTypeOfMaintenance(const utf8string& name)
{
    return this->amt->AddTypeOfMaintenance(name);
}

/// Update the name of a maintenance type
/// @param name the current name
/// @param newName the new name of the maintenance type
/// @return true if updated/same otherwise false
bool AMTAPI::UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType)
{
    return this->amt->UpdateTypesOfMaintenance(type, newType);
}

/// List available types of maintenance
/// @return the list of available maintenance types
std::shared_ptr<std::vector<utf8string>>
AMTAPI::ListAllTypesOfMaintenance()
{
    return this->amt->ListAllTypesOfMaintenance();
}

/// Create and persist a new maintenance task
/// @return a vehicle with a persisted ID
std::shared_ptr<MaintenanceTask>
AMTAPI::CreateMaintenanceTask(int vehicleID)
{
    return this->amt->CreateMaintenanceTask(vehicleID);
}

/// Persists the maintenance task values to storage
/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
/// return true if update/add otherwise false
bool AMTAPI::UpdateMaintenanceTask(MaintenanceTask& task)
{
    return this->amt->UpdateMaintenanceTask(task);
}

/// Persists the maintenance task values to storage
/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
/// @param properties which properties to persist
/// return true if update/add otherwise false
bool AMTAPI::UpdateMaintenanceTask(MaintenanceTask& task,
	MaintenanceTask::Properties properties)
{
    return this->amt->UpdateMaintenanceTask(task, properties);
}

/// Remove and no longer persist a maintenance task
/// @return true if task removed/not found otherwise false
bool AMTAPI::DeleteMaintenanceTask(int taskID)
{
    return this->amt->DeleteMaintenanceTask(taskID);
}

/// List the entire maintenance history of the vehicle
/// @param vehicleID the vehicle's ID
/// @return a list of maintenance tasks associated with the vehicle
std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask>>>
AMTAPI::ListVehicleMaintenanceHistory(int vehicleID)
{
	return this->amt->ListVehicleMaintenanceHistory(vehicleID);
}

/// List the entire maintenance history of the vehicle
/// @param vehicleID the vehicle's ID
/// @param startDate the earliest maintenance history date inclusive
/// @param endDate the latest maintenance history date inclusive
/// @return a list of maintenance tasks associated with the vehicle
std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask>>>
AMTAPI::ListVehicleMaintenanceHistory(int vehicleID, int startDate, int endDate)
{
	return this->amt->ListVehicleMaintenanceHistory(vehicleID, startDate, endDate);
}
