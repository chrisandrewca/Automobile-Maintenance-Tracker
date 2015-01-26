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
};

}

#endif