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
bool
API::AddTypeOfVehicle(const utf8string& name)
{
	return this->dataStore->AddTypeOfVehicle(name);
}

bool
API::UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName)
{
	return this->dataStore->UpdateTypesOfVehicles(name, newName);
}

std::unique_ptr<std::vector<utf8string> >
API::ListAllTypesOfVehicles()
{
	return this->dataStore->ListAllTypesOfVehicles();
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
const std::vector<utf8string>& userDefinedProperties)
{
	return false;
}

bool
API::AddTypeOfMaintenance(const utf8string& type)
{
	return false;
}

bool
API::UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType)
{
	return false;
}

std::vector<utf8string>
API::ListAllTypesOfMaintenance()
{
	return std::vector<utf8string>();
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
