#include "AutomobileMaintenanceTracker.hpp"
#include "Database.hpp"
using namespace AMT;

API::API(API::DataStoreOption dataStore)
{
	switch (dataStore)
	{
	case API::DataStoreOption::Database:
		this->dataStore = new Database;
		this->dataStore->Open("auto_maintenance_tracker", std::string());
		break;
	}
}

API::~API()
{
	if (this->dataStore)
		delete this->dataStore;
}

// unique ptr to vector vs copy list?

/// Track a new type of vehicle
/// @param name the new type of vehicle
/// @return true if added or already added otherwise false
bool
API::AddTypeOfVehicle(const std::string& name)
{
	return this->dataStore->AddTypeOfVehicle(name);
}

bool
API::UpdateTypesOfVehicles(const std::string& type, const std::string& newType)
{
	return false;
}

std::vector<std::string>
API::ListAllTypesOfVehicles()
{
	return std::vector<std::string>();
}

// !!! use unique ptrs
std::unique_ptr<Vehicle>
API::CreateVehicle()
{
	return std::unique_ptr<Vehicle>();
}

bool
API::DeleteVehicle(Vehicle& vehicle)
{
	return false;
}

std::vector<std::unique_ptr<Vehicle> >
API::ListAllVehicles() const
{
	return std::vector<std::unique_ptr<Vehicle> >();
}

std::vector<std::unique_ptr<Vehicle> >
API::FindVehicles(VehicleProperties properties, const Vehicle& values) const
{
	return std::vector<std::unique_ptr<Vehicle> >();
}

std::unique_ptr<Vehicle>
API::GetVehicle(int vehicleId) const
{
	return std::unique_ptr<Vehicle>();
}

bool
API::UpdateVehicle(Vehicle& vehicle)
{
	return false;
}

bool
API::UpdateVehicle(Vehicle& vehicle,
VehicleProperties properties,
const std::vector<std::string>& userDefinedProperties)
{
	return false;
}

bool
API::AddTypeOfMaintenance(const std::string& type)
{
	return false;
}

bool
API::UpdateTypesOfMaintenance(const std::string& type, const std::string& newType)
{
	return false;
}

std::vector<std::string>
API::ListAllTypesOfMaintenance()
{
	return std::vector<std::string>();
}

std::unique_ptr<MaintenanceTask>
API::CreateMaintenanceTask(int vehicleID)
{
	return std::unique_ptr<MaintenanceTask>();
}

bool
API::UpdateMaintenanceTask(MaintenanceTask& task)
{
	return false;
}

bool
API::UpdateMaintenanceTask(MaintenanceTask& task,
MaintenanceTaskProperties properties)
{
	return false;
}

bool
API::DeleteMaintenanceTask(MaintenanceTask& task)
{
	return false;
}
