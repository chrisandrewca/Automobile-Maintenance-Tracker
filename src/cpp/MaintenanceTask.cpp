#include "AutomobileMaintenanceTracker.hpp"
using namespace AMT;

MaintenanceTask::MaintenanceTask(int ID, int vehicleID) :
	id(ID),
    vehicleId(vehicleID),
	type(),
    date(0)
{
}

int& MaintenanceTask::GetID()
{
	return this->id;
}

void MaintenanceTask::SetID(int ID)
{
	this->id = ID;
}

int& MaintenanceTask::VehicleID()
{
	return this->vehicleId;
}

void MaintenanceTask::SetVehicleID(int ID)
{
	this->vehicleId = ID;
}

utf8string& MaintenanceTask::GetType()
{
	return this->type;
}

void MaintenanceTask::SetType(const utf8string& type)
{
	this->type = type;
}

int& MaintenanceTask::GetDate()
{
	return this->date;
}

void MaintenanceTask::SetDate(int date)
{
	this->date = date;
}
