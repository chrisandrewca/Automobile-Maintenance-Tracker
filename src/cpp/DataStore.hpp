#ifndef _AMT_DATA_STORE_HPP_
#define _AMT_DATA_STORE_HPP_

#include "AutomobileMaintenanceTracker.hpp"
#include <unordered_map>
#include <set>

namespace AMT
{

class DataStore
{
public:
	virtual bool UpdateVehicle(Vehicle& vehicle) = 0;
};

}

#endif