#ifndef _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_
#define _AUTOMOBILE_MAINTENANCE_TRACKER_HPP_

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

/// !!! what are the CRUD API semantics think ease of use
// if vehicle updated its own properties it would need a ref to the datastore, an injected ctor,
// and the user would need to call V.save with these flags anyways (because client cant change
// a const ref), so the API handles it
// flags could be as large or fine grained as needed to cover different types of objects

namespace AMT
{
// returning/accept utf8 strings
typedef std::string utf8string;

class Vehicle
{
public:
	enum class Properties
	{
		All = 0,
		Type = 1,
		Make = 2,
		Model = 4, // re-org
		Year = 8,
		Odometer = 16,
		UserDefined = 32
	};

    Vehicle(int id = -1);

	int& GetID();

	int& GetYear();

	int& GetOdometer();

	utf8string& GetType();

	utf8string& GetMake();

	utf8string& GetModel();

	void AddProperty(const utf8string& property, const utf8string& value);

	utf8string& GetProperty(const utf8string& name);

	std::vector<utf8string>& GetPropertyNames() const;

private:
    int id;
    int year;
    int odometer;
    utf8string type;
    utf8string make;
    utf8string model;
    std::unordered_map<utf8string, utf8string> properties;
};

class MaintenanceTask
{
public:
	enum class Properties
	{
		All = 0,
		Type = 1, // re-org
		Date = 2,
		VehicleID = 4
	};

	MaintenanceTask(int id = -1);

	int& GetID();

	int& VehicleID();

	utf8string& GetType();

	int& GetDate();

private:
	int id;
	int vehicleId;
	utf8string type;
	int date;
};

class APIBase
{
public:
	virtual ~APIBase(){}

	/// Track a new type of vehicle
	/// @param name the new type of vehicle
	/// @return true if added or already added otherwise false
	virtual bool AddTypeOfVehicle(const utf8string& name) = 0;

	/// Update the name of a vehicle type
	/// @param name the current name
	/// @param newName the new name of the vehicle type
	/// @return true if updated/same otherwise false
	virtual bool
		UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName) = 0;

	/// List available types of vehicles
	/// @return the list of available vehicles
	virtual std::unique_ptr<std::vector<utf8string> >
		ListAllTypesOfVehicles() = 0;

	/// Create and persist a new vehicle
	/// @return a vehicle with a persisted ID
	virtual std::unique_ptr<Vehicle> CreateVehicle() = 0;

	/// Remove and no longer persist a vehicle
	/// @return true if vehicle removed/not found otherwise false
	virtual bool DeleteVehicle(Vehicle& vehicle) = 0;

	/// List all available vehicles
	/// @return the list of all available vehicles
	virtual std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
		ListAllVehicles() = 0;

	/// Search for vehicles matching the supplied vehicle properties
	/// @param properties the vehicle properties to match
	/// @param values the values of the vehicle properties to match
	virtual std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
		FindVehicles(Vehicle::Properties properties, const Vehicle& values) = 0;

	/// Get the vehicle with the supplied ID
	/// @param vehicleId the vehicle ID to match
	/// @return a vehicle with an ID > -1 if successful otherwise ID == -1
	virtual std::unique_ptr<Vehicle> GetVehicle(int vehicleId) = 0;

	/// Persists the vehicle values to storage
	/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
	/// return true if update/add otherwise false
	virtual bool UpdateVehicle(Vehicle& vehicle) = 0;

	/// Persists the vehicle values to storage
	/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
	/// @param properties which properties to persist
	/// @param userDefinedProperties which user defined properties to persist; ignored when Properties::All
	/// return true if update/add otherwise false
	virtual bool UpdateVehicle(Vehicle& vehicle,
					Vehicle::Properties properties,
						const std::vector<utf8string>& userDefinedProperties
							= std::vector<utf8string>()) = 0;

	/// Track a new type of maintenance
	/// @param name new maintenance name
	/// @return true if added otherwise false
	virtual bool AddTypeOfMaintenance(const utf8string& name) = 0;

	/// Update the name of a maintenance type
	/// @param name the current name
	/// @param newName the new name of the maintenance type
	/// @return true if updated/same otherwise false
	virtual bool
		UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType) = 0;

	/// List available types of maintenance
	/// @return the list of available maintenance types
	virtual std::unique_ptr<std::vector<utf8string>>
		ListAllTypesOfMaintenance() = 0;

	/// Create and persist a new maintenance task
	/// @return a vehicle with a persisted ID
	virtual std::unique_ptr<MaintenanceTask>
		CreateMaintenanceTask(int vehicleID) = 0;

	/// Persists the maintenance task values to storage
	/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
	/// return true if update/add otherwise false
	virtual bool
		UpdateMaintenanceTask(MaintenanceTask& task) = 0;

	/// Persists the maintenance task values to storage
	/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
	/// @param properties which properties to persist
	/// return true if update/add otherwise false
	virtual bool
		UpdateMaintenanceTask(MaintenanceTask& task,
			MaintenanceTask::Properties properties) = 0;

	/// Remove and no longer persist a maintenance task
	/// @return true if task removed/not found otherwise false
	virtual bool DeleteMaintenanceTask(MaintenanceTask& task) = 0;
};

class AMTAPI : public APIBase
{
public:
	struct Result
	{
	};

	enum class DataStoreOption { Database };

	AMTAPI(DataStoreOption dataStore);
	~AMTAPI();

	/// Track a new type of vehicle
	/// @param name the new type of vehicle
	/// @return true if added or already added otherwise false
	bool AddTypeOfVehicle(const utf8string& name) override;

	/// Update the name of a vehicle type
	/// @param name the current name
	/// @param newName the new name of the vehicle type
	/// @return true if updated/same otherwise false
	bool UpdateTypesOfVehicles(const utf8string& name, const utf8string& newName) override;

	/// List available types of vehicles
	/// @return the list of available vehicles
	std::unique_ptr<std::vector<utf8string> >
		ListAllTypesOfVehicles() override;

	/// Create and persist a new vehicle
	/// @return a vehicle with a persisted ID
	std::unique_ptr<Vehicle> CreateVehicle() override;

	/// Remove and no longer persist a vehicle
	/// @return true if vehicle removed/not found otherwise false
	bool DeleteVehicle(Vehicle& vehicle) override;

	/// List all available vehicles
	/// @return the list of all available vehicles
	std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
		ListAllVehicles() override;

	/// Search for vehicles matching the supplied vehicle properties
	/// @param properties the vehicle properties to match
	/// @param values the values of the vehicle properties to match
	std::unique_ptr<std::vector<std::unique_ptr<Vehicle>>>
		FindVehicles(Vehicle::Properties properties, const Vehicle& values) override;

	/// Get the vehicle with the supplied ID
	/// @param vehicleId the vehicle ID to match
	/// @return a vehicle with an ID > -1 if successful otherwise ID == -1
	std::unique_ptr<Vehicle> GetVehicle(int vehicleId) override;

	/// Persists the vehicle values to storage
	/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
	/// return true if update/add otherwise false
	bool UpdateVehicle(Vehicle& vehicle) override;

	/// Persists the vehicle values to storage
	/// @param vehicle the vehicle to update/add; vehicle ID changes if not constructed through CreateVehicle()
	/// @param properties which properties to persist
	/// @param userDefinedProperties which user defined properties to persist; ignored when Properties::All
	/// return true if update/add otherwise false
	bool UpdateVehicle(Vehicle& vehicle,
			Vehicle::Properties properties,
			const std::vector<utf8string>& userDefinedProperties
				= std::vector<utf8string>()) override;

	/// Track a new type of maintenance
	/// @param name new maintenance name
	/// @return true if added otherwise false
	bool AddTypeOfMaintenance(const utf8string& name) override;

	/// Update the name of a maintenance type
	/// @param name the current name
	/// @param newName the new name of the maintenance type
	/// @return true if updated/same otherwise false
	bool UpdateTypesOfMaintenance(const utf8string& type, const utf8string& newType) override;

	/// List available types of maintenance
	/// @return the list of available maintenance types
	std::unique_ptr<std::vector<utf8string>>
		ListAllTypesOfMaintenance() override;

	/// Create and persist a new maintenance task
	/// @return a vehicle with a persisted ID
	std::unique_ptr<MaintenanceTask>
		CreateMaintenanceTask(int vehicleID) override;

	/// Persists the maintenance task values to storage
	/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
	/// return true if update/add otherwise false
	bool UpdateMaintenanceTask(MaintenanceTask& task) override;

	/// Persists the maintenance task values to storage
	/// @param task the task to update/add; task ID changes if not constructed through CreateMaintenanceTask()
	/// @param properties which properties to persist
	/// return true if update/add otherwise false
	bool UpdateMaintenanceTask(MaintenanceTask& task,
			MaintenanceTask::Properties properties) override;

	/// Remove and no longer persist a maintenance task
	/// @return true if task removed/not found otherwise false
	bool DeleteMaintenanceTask(MaintenanceTask& task) override;

private:
	class Implementation;
	Implementation* amt;
};

}

#endif
