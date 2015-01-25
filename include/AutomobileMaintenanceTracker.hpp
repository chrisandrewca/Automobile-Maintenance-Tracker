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

class VehicleType
{
	enum class VehicleTypeProperty
	{
		Name
	};

	typedef std::set<VehicleTypeProperty> VehicleTypePropertyChangeSet;

	int GetID() const;
	void SetID(int value);

	std::string GetName() const;
	void SetName(const std::string& value);
};

template <typename T>
class DataStoreEntity
{
public:
	virtual const std::set<T>& GetPropertyChangeSet() const = 0;
};

enum class VehicleProperty
{
	Type,
	Make,
	Model,
	Year,
	Odometer,
	UserDefined
};

typedef std::set<VehicleProperty> VehiclePropetyChangeSet;

class Vehicle : public DataStoreEntity<VehicleProperty>
{
public:
	Vehicle(); // id = -1

	std::unique_ptr<VehicleType> GetType() const;
	void SetType(VehicleType& value);

	std::string& GetMake() const;
	void SetMake(const std::string& value);

	std::string& GetModel() const;
	void SetModel(const std::string& value);

	int GetYear();
	void SetYear(int value);

	int GetOdometer();
	void SetOdometer(int value);

	int GetID() const;
	//void SetMaintenanceID(int value);

	std::string& GetProperty(const std::string& name, std::string& value) const;
	void SetProperty(const std::string& name, const std::string& value);

	const VehiclePropetyChangeSet& GetPropertyChangeSet() const override;
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
	std::vector<std::unique_ptr<Vehicle> > FindVehicles(
		const std::vector<VehicleProperty>& vehicleProperties,
		const std::vector<std::string>& userDefinedProperties = std::vector<std::string>()) const;
	std::vector<std::unique_ptr<Vehicle> > GetVehicle(int vehicleId) const;
	std::vector<std::unique_ptr<Vehicle> > ListAllVehicles() const;
	bool UpdateVehicle(Vehicle& vehicle);
	bool UpdateVehicle(Vehicle& vehicle, VehiclePropetyChangeSet& propertyChanges);

private:
	DataStore* dataStore;
};

}

#endif