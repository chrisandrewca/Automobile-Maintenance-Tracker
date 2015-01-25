#ifndef _AMT_DATA_STORE_HPP_
#define _AMT_DATA_STORE_HPP_

#include "AutomobileMaintenanceTracker.hpp"

namespace AMT
{

class DataStore
{
public:
	virtual bool UpdateVehicle(Vehicle& vehicle) = 0;
};

}

#endif