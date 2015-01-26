#ifndef _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_
#define _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_

#include <string>
#include <memory>
#include <vector>

/// !!! what are the CRUD API semantics think ease of use

namespace AMT
{
	// returning/accept utf8 strings
typedef std::string utf8string;
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
	utf8string& GetType();

	utf8string& GetMake();

	utf8string& GetModel();

	int& GetYear();

	int& GetOdometer();

	int GetID();

	void AddProperty(const utf8string& property, const utf8string& value);

	// onUpdate:: if property not in DataStore add it
	utf8string& GetProperty(const utf8string& name);

	std::vector<utf8string>& GetPropertyNames() const;
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

	utf8string& GetType();

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

	/// !!! unique ptr to vector vs copy list?

	/// Track a new type of vehicle
	/// @param name the new type of vehicle
	/// @return true if added or already added otherwise false
	bool
	AddTypeOfVehicle(const utf8string& name);
	
	/// Update the name of a vehicle type
	/// @param name the current name
	/// @param newName the new name of the vehicle type
	/// @return true if updated or the same otherwise false
	bool
	UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName);
	
	/// !!! unique_ptr vs vectory copy
	/// List available types of vehicles
	/// @return the list of available vehicles
	std::unique_ptr<std::vector<utf8string> >
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
				  const std::vector<utf8string>& userDefinedProperties = std::vector<utf8string>());

	bool
	AddTypeOfMaintenance(const utf8string& type);
	
	bool
	UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType);
	
	std::vector<utf8string>
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