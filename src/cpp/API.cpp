#include "AutomobileMaintenanceTracker.hpp"
#include "Database.hpp"
using namespace AMT;

API::API(API::DataStoreOption dataStore)
{
	switch (dataStore)
	{
	case API::DataStoreOption::Database:
		this->dataStore = new Database;
		break;
	}
}

API::~API()
{
	if (this->dataStore)
		delete this->dataStore;
}