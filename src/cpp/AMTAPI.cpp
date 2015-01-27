#include "AutomobileMaintenanceTracker.hpp"
#include "Database.hpp"
#include "AMTAPI_Impl.hpp"
using namespace AMT;

class AMTAPI::Implementation
{
public:
	std::shared_ptr<APIBase> api;
};

AMTAPI::AMTAPI(AMTAPI::DataStoreOption dataStore) :
	amt(new Implementation)
{
	switch (dataStore)
	{
	case DataStoreOption::Database:
		std::string errMsg;
		auto* database = new Database;
		database->Open("auto_maintenance_tracker", errMsg);
		amt->api.reset(database);
		break;
	}
}

AMTAPI::~AMTAPI()
{
	if (this->amt)
		delete this->amt;
}


bool
AMTAPI::AddTypeOfVehicle(const utf8string& name)
{
	return false;
}

bool
AMTAPI::UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName)
{
	return false;
}

std::unique_ptr<std::vector<utf8string> >
AMTAPI::ListAllTypesOfVehicles()
{
	auto* v = new std::vector<utf8string>();
	return std::unique_ptr<std::vector<utf8string>>(v);
}

std::unique_ptr<Vehicle>
AMTAPI::CreateVehicle()
{
	return std::unique_ptr<Vehicle>(new Vehicle);
}

bool
AMTAPI::DeleteVehicle(Vehicle& vehicle)
{
	return true;
}

/// List all available vehicles
/// @return the list of all available vehicles
std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
AMTAPI::ListAllVehicles()
{
	return std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>();
}

/// Search for vehicles matching the supplied vehicle properties
/// @param properties the vehicle properties to match
/// @param values the values of the vehicle properties to match
std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
AMTAPI::FindVehicles(Vehicle::Properties properties, const Vehicle& values)
{
	auto v = std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>();
	return v;
}

/// Get the vehicle with the supplied ID
/// @param vehicleId the vehicle ID to match
/// @return a vehicle with an ID > -1 if successful otherwise ID == -1
std::unique_ptr<Vehicle> AMTAPI::GetVehicle(int vehicleId)
{
	return std::unique_ptr<Vehicle>(new Vehicle);
}

/// Persists the vehicle values to storage
/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
/// return true if update/add otherwise false
bool AMTAPI::UpdateVehicle(Vehicle& vehicle)
{
	return false;
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
	return false;
}

/// Track a new type of maintenance
/// @param name new maintenance name
/// @return true if added otherwise false
bool AMTAPI::AddTypeOfMaintenance(const utf8string& name)
{
	return false;
}

/// Update the name of a maintenance type
/// @param name the current name
/// @param newName the new name of the maintenance type
/// @return true if updated/same otherwise false
bool AMTAPI::UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType)
{
	return false;
}

/// List available types of maintenance
/// @return the list of available maintenance types
std::unique_ptr<std::vector<utf8string>>
AMTAPI::ListAllTypesOfMaintenance()
{
	return std::unique_ptr<std::vector<utf8string>>(new std::vector<utf8string>());
}

/// Create and persist a new maintenance task
/// @return a vehicle with a persisted ID
std::unique_ptr<MaintenanceTask>
AMTAPI::CreateMaintenanceTask(int vehicleID)
{
	return std::unique_ptr<MaintenanceTask>(new MaintenanceTask);
}

/// Persists the maintenance task values to storage
/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
/// return true if update/add otherwise false
bool AMTAPI::UpdateMaintenanceTask(MaintenanceTask& task)
{
	return false;
}

/// Persists the maintenance task values to storage
/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
/// @param properties which properties to persist
/// return true if update/add otherwise false
bool AMTAPI::UpdateMaintenanceTask(MaintenanceTask& task,
	MaintenanceTask::Properties properties)
{
	return false;
}

/// Remove and no longer persist a maintenance task
/// @return true if task removed/not found otherwise false
bool AMTAPI::DeleteMaintenanceTask(MaintenanceTask& task)
{
	return false;
}