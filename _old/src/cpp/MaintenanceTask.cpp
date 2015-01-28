#include "AutomobileMaintenanceTracker.hpp"
using namespace AMT;

MaintenanceTask::MaintenanceTask(int id) :
	id(id),
	vehicleId(),
	type(),
	date()
{
}

int& MaintenanceTask::GetID()
{
	return this->id;
}

int& MaintenanceTask::VehicleID()
{
	return this->vehicleId;
}

utf8string& MaintenanceTask::GetType()
{
	return this->type;
}

int& MaintenanceTask::GetDate()
{
	return this->date;
}
