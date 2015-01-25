#ifndef _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_
#define _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <set>

// transform DB data into API data
//class Vehicle
//{
//public:
//	std::string type;
//private:
//	int id;
//	int type;
//	std::string model;
//	int year;
//	int odemeter;
//};

/// !!! what are the CRUD API semantics think ease of use

// keeps track of changes via list of columns "TYPE"
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

// could this just be API calls?
//class MaintenanceType
//{
//	enum class MaintenanceTypeProperty
//	{
//		Name
//	};
//
//	typedef std::set<MaintenanceTypeProperty> MaintenanceTypePropertyChangeSet;
//
//	int GetID() const;
//	void SetID(int value);
//
//	std::string GetName() const;
//	void SetName(const std::string& value);
//};

enum class MaintenanceProperties
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

// can be pure API calls using other classes
//class VehicleApplicableMaintenance;

// can be pure API calls using other classes
//class VehicleUserDefinedField

// API creates objects
// API has ptr to database, database inherits from "persistance layer"
// API injects database:PL ptr into Vehicle
// Vehicle calls PL.prepChange(id, property);
// Vehicle calls PL.submitChanges(id);
// PL has multimap<id,property>, database:PL constructs SQL query with if/else/Get


// 2 interfaces- 1 to accept full Vehicle and submit
			// - 2 to accept vehicle plus changeset
class DataStore;
class API
{
public:
	enum class DataStoreOption { Database };

	API(DataStoreOption dataStore);
	~API();

	// unique ptr to vector vs copy list?
	bool
	AddTypeOfVehicle(const std::string& type);
	
	bool
	UpdateTypesOfVehicles(const std::string& type, const std::string& newType);
	
	std::vector<std::string>
	ListAllTypesOfVehicles();

	// !!! use unique ptrs
	std::unique_ptr<Vehicle>
	CreateVehicle(); // not really needed? we can handle -1 id case
	
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
						  MaintenanceProperties properties);
	
	bool
	DeleteMaintenanceTask(MaintenanceTask& task);

private:
	DataStore* dataStore;
};

}

#endif