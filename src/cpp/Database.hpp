#ifndef _AMT_DATABASE_HPP_
#define _AMT_DATABASE_HPP_

#include "AutomobileMaintenanceTracker.hpp"
#include "sqlite/sqlite3.h"

namespace AMT
{

class Database : public APIBase
{
public:
	Database();
	~Database();

	bool Open(const std::string& name, std::string& errorMessage);

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
	std::shared_ptr<std::vector<utf8string> >
		ListAllTypesOfVehicles() override;

	/// Create and persist a new vehicle
	/// @return a vehicle with a persisted ID
	std::shared_ptr<Vehicle> CreateVehicle() override;

	/// Remove and no longer persist a vehicle
	/// @return true if vehicle removed/not found otherwise false
	bool DeleteVehicle(int vehicleID) override;

	/// List all available vehicles
	/// @return the list of all available vehicles
	std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
		ListAllVehicles() override;

	/// Search for vehicles matching the supplied vehicle properties
	/// @param properties the vehicle properties to match
	/// @param values the values of the vehicle properties to match
	std::shared_ptr<std::vector<std::shared_ptr<Vehicle> > >
		FindVehicles(Vehicle::Properties properties, const Vehicle& values) override;

	/// Get the vehicle with the supplied ID
	/// @param vehicleId the vehicle ID to match
	/// @return a vehicle with an ID > -1 if successful otherwise ID == -1
    std::shared_ptr<Vehicle> GetVehicle(int vehicleID) override;

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
	std::shared_ptr<std::vector<utf8string> >
		ListAllTypesOfMaintenance() override;

	/// Create and persist a new maintenance task
	/// @return a vehicle with a persisted ID
	std::shared_ptr<MaintenanceTask>
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
	bool DeleteMaintenanceTask(int maintenanceID) override;

	/// List the entire maintenance history of the vehicle
	/// @param vehicleID the vehicle's ID
	/// @return a list of maintenance tasks associated with the vehicle
	std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
		ListVehicleMaintenanceHistory(int vehicleID) override;

	/// List the entire maintenance history of the vehicle
	/// @param vehicleID the vehicle's ID
	/// @param startDate the earliest maintenance history date inclusive
	/// @param endDate the latest maintenance history date inclusive
	/// @return a list of maintenance tasks associated with the vehicle
	std::shared_ptr<std::vector<std::shared_ptr<MaintenanceTask> > >
		ListVehicleMaintenanceHistory(int vehicleID, int startDate, int endDate) override;

private:
    sqlite3* sqlite;
    // per function -- takeout anythin really common
    std::vector<sqlite3_stmt*> preparedStatements;

    int PrepareQuery(const std::string& queryText, sqlite3_stmt** preparedQuery);
    int RetrieveVehiclePropsAndValues(Vehicle& vehicle, std::unordered_map<utf8string, utf8string>& propValMap);
	bool Setup(const char* databaseName, std::string& errorMessage);
};

}

#endif
