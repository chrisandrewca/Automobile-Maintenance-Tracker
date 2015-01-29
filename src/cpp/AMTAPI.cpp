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

bool AMTAPI::AddTypeOfVehicle(const utf8string& name)
{
	return this->amt->AddTypeOfVehicle(name);
}

bool AMTAPI::UpdateTypesOfVehicles(const utf8string& name,
								   const utf8string& newName)
{
	return this->amt->UpdateTypesOfVehicles(name, newName);
}

std::shared_ptr<std::vector<utf8string> > AMTAPI::ListAllTypesOfVehicles()
{
    return this->amt->ListAllTypesOfVehicles();
}

std::shared_ptr<Vehicle> AMTAPI::CreateVehicle()
{
    return this->amt->CreateVehicle();
}

bool AMTAPI::DeleteVehicle(int vehicleID)
{
    return this->amt->DeleteVehicle(vehicleID);
}

std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
	AMTAPI::ListAllVehicles()
{
    return this->amt->ListAllVehicles();
}

std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
	AMTAPI::FindVehicles(Vehicle::Properties properties, const Vehicle& values)
{
    return this->amt->FindVehicles(properties, values);
}

std::shared_ptr<Vehicle> AMTAPI::GetVehicle(int vehicleID)
{
    return this->amt->GetVehicle(vehicleID);
}

bool AMTAPI::UpdateVehicle(Vehicle& vehicle)
{
    return this->amt->UpdateVehicle(vehicle);
}

bool AMTAPI::UpdateVehicle(Vehicle& vehicle,
						   Vehicle::Properties properties,
						   const std::vector<utf8string>& userDefinedProperties)
{
    return this->amt->UpdateVehicle(vehicle, properties, userDefinedProperties);
}

bool AMTAPI::AddTypeOfMaintenance(const utf8string& name)
{
    return this->amt->AddTypeOfMaintenance(name);
}

bool AMTAPI::UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType)
{
    return this->amt->UpdateTypesOfMaintenance(type, newType);
}

std::shared_ptr<std::vector<utf8string> > AMTAPI::ListAllTypesOfMaintenance()
{
    return this->amt->ListAllTypesOfMaintenance();
}

std::shared_ptr<MaintenanceTask> AMTAPI::CreateMaintenanceTask(int vehicleID)
{
    return this->amt->CreateMaintenanceTask(vehicleID);
}

bool AMTAPI::UpdateMaintenanceTask(MaintenanceTask& task)
{
    return this->amt->UpdateMaintenanceTask(task);
}

bool AMTAPI::UpdateMaintenanceTask(MaintenanceTask& task,
	MaintenanceTask::Properties properties)
{
    return this->amt->UpdateMaintenanceTask(task, properties);
}

bool AMTAPI::DeleteMaintenanceTask(int taskID)
{
    return this->amt->DeleteMaintenanceTask(taskID);
}

std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
	AMTAPI::ListVehicleMaintenanceHistory(int vehicleID)
{
	return this->amt->ListVehicleMaintenanceHistory(vehicleID);
}

std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
	AMTAPI::ListVehicleMaintenanceHistory(int vehicleID, int startDate, int endDate)
{
	return this->amt->ListVehicleMaintenanceHistory(vehicleID, startDate, endDate);
}
