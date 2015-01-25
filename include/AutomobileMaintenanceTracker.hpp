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

class Vehicle/* : public DataStoreEntity<VehicleProperties>*/
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

	int GetID() const;

	// onUpdate:: if property not in DataStore add it
	void AddProperty(const std::string& property, const std::string& value);

	std::string& GetProperty(const std::string& name);

	std::vector<std::string>& GetPropertyNames() const;
};

class MaintenanceType
{
	enum class MaintenanceTypeProperty
	{
		Name
	};

	typedef std::set<MaintenanceTypeProperty> MaintenanceTypePropertyChangeSet;

	int GetID() const;
	void SetID(int value);

	std::string GetName() const;
	void SetName(const std::string& value);
};

class MaintenanceTask
{
public:
	MaintenanceTask();

	std::unique_ptr<MaintenanceType> GetType() const;
	void SetType(MaintenanceType& value); // API query maintenance types
	
	int GetDate() const;
	void SetDate(int value);

	int GetID() const;

	int VehicleID() const; // "lazy loading"
	void SetVehicleID(int value);

	enum class MaintenanceTaskProperty
	{
		Type,
		Date,
		VehicleID
	};
	typedef std::set<MaintenanceTaskProperty> MaintenanceTaskPropertyChangeSet;
	const MaintenanceTaskPropertyChangeSet& GetChangeSet() const;
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

	// !!! use unique ptrs
	std::unique_ptr<Vehicle> CreateVehicle(); // not really needed? we can handle -1 id case
	
	bool DeleteVehicle(Vehicle& vehicle);
	
	std::vector<std::unique_ptr<Vehicle> > FindVehicles(VehicleProperties vehicleProperties,
		const Vehicle& vehicleValues) const;
	
	std::unique_ptr<Vehicle> GetVehicle(int vehicleId) const;
	
	std::vector<std::unique_ptr<Vehicle> > ListAllVehicles() const;

	// unique ptr to vector vs copy list?
	std::vector<std::string> ListAllVehicleTypes();
	std::vector<std::string> ListUserDefinedVehiclePropertiesByType(const std::string& vehicleType);
	
	bool UpdateVehicle(Vehicle& vehicle, VehicleProperties properties,
		const std::vector<std::string>& userDefinedProperties = std::vector<std::string>());

private:
	DataStore* dataStore;
};

}

#endif