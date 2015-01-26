#ifndef _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_
#define _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_

#include <string>
#include <memory>
#include <vector>

/// !!! what are the CRUD API semantics think ease of use

namespace AMT
{

// if vehicle updated its own properties it would need a ref to the datastore, an injected ctor,
	// and the user would need to call V.save with these flags anyways (because client cant change
	// a const ref), so the API handles it
	// flags could be as large or fine grained as needed to cover different types of objects
enum class VehicleProperties
{
	All			=  0,
	Type		=  1,
	Make		=  2,
	Model		=  4,
	Year		=  8,
	Odometer	= 16,
	UserDefined = 32
};

class Vehicle
{
public:
	Vehicle(); // id = -1

	// api returns unique ptr to class
		// class returns reference and cleans up ptr via dtor/smart ptr
	// onUpdate:: if type not in DataStore add it
	std::string& GetType();

	std::string& GetMake();

	std::string& GetModel();

	int& GetYear();

	int& GetOdometer();

	int GetID();

	void AddProperty(const std::string& property, const std::string& value);

	// onUpdate:: if property not in DataStore add it
	std::string& GetProperty(const std::string& name);

	std::vector<std::string>& GetPropertyNames() const;
};

enum class MaintenanceTaskProperties
{
	All		  = 0,
	Type	  = 1,
	Date	  = 2,
	VehicleID = 4
};

class MaintenanceTask
{
public:
	MaintenanceTask(); // id -1

	std::string& GetType();

	int& GetDate();

	int GetID();

	int& VehicleID(); // "lazy loading"
};

class DataStore;
class API
{
public:
	enum class DataStoreOption { Database };

	API(DataStoreOption dataStore);
	~API();

	// unique ptr to vector vs copy list?
	bool
	AddTypeOfVehicle(const std::string& name);
	
	bool
	UpdateTypesOfVehicles(const std::string& type, const std::string& newType);
	
	std::vector<std::string>
	ListAllTypesOfVehicles();

	// !!! use unique ptrs
	std::unique_ptr<Vehicle>
	CreateVehicle();
	
	bool
	DeleteVehicle(Vehicle& vehicle);

	std::vector<std::unique_ptr<Vehicle> >
	ListAllVehicles() const;
	
	std::vector<std::unique_ptr<Vehicle> >
	FindVehicles(VehicleProperties properties, const Vehicle& values) const;

	std::unique_ptr<Vehicle>
	GetVehicle(int vehicleId) const;
	
	bool
	UpdateVehicle(Vehicle& vehicle);

	bool
	UpdateVehicle(Vehicle& vehicle,
				  VehicleProperties properties,
				  const std::vector<std::string>& userDefinedProperties = std::vector<std::string>());

	bool
	AddTypeOfMaintenance(const std::string& type);
	
	bool
	UpdateTypesOfMaintenance(const std::string& type, const std::string& newType);
	
	std::vector<std::string>
	ListAllTypesOfMaintenance();
	
	std::unique_ptr<MaintenanceTask>
	CreateMaintenanceTask(int vehicleID);

	bool
	UpdateMaintenanceTask(MaintenanceTask& task);

	bool
	UpdateMaintenanceTask(MaintenanceTask& task,
						  MaintenanceTaskProperties properties);
	
	bool
	DeleteMaintenanceTask(MaintenanceTask& task);

private:
	DataStore* dataStore;
};

}

#endif