#ifndef _AMT_DATA_STORE_HPP_
#define _AMT_DATA_STORE_HPP_

#include "AutomobileMaintenanceTracker.hpp"

namespace AMT
{

class DataStore
{
public:
	virtual bool Open(const std::string& name, std::string& errorMessage) = 0;

	/// Track a new type of vehicle
	/// @param name the new type of vehicle
	/// @return true if added or already added otherwise false
	virtual bool
	DataStore::AddTypeOfVehicle(const std::string& name) = 0;

	/// Update the name of a vehicle type
	/// @param name the current name
	/// @param newName the new name of the vehicle type
	/// @return true if updated or the same otherwise false
	virtual bool
	UpdateTypesOfVehicles(const std::string& name, const std::string& newName) = 0;

	/// List available types of vehicles
	/// @return the list of available vehicles
	virtual std::unique_ptr<std::vector<std::string> >
	ListAllTypesOfVehicles() = 0;
};

}

#endif