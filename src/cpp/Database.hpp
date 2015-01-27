#ifndef _AMT_DATABASE_HPP_
#define _AMT_DATABASE_HPP_

//#include "DataStore.hpp"
#include "AutomobileMaintenanceTracker.hpp"
#include "sqlite/sqlite3.h"
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

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
	static const int TblVehicleType_ColName;
	static const int TblMaintenanceType_ColName;

	static const int TblVehicle_ColID;
	static const int TblVehicle_ColType;
	static const int TblVehicle_ColMake;
	static const int TblVehicle_ColModel;
	static const int TblVehicle_ColYear;
	static const int TblVehicle_ColOdometer;

	static const int TblMaintenance_ColID;
	static const int TblMaintenance_ColVehicleID;
	static const int TblMaintenance_ColType;
	static const int TblMaintenance_ColDate;

	static const int TblVehicleApplicableMaintenance_ColVehicleType;
	static const int TblVehicleApplicableMaintenance_ColMaintenanceType;

	static const int TblVehicleUserDefinedField_ColName;
	static const int TblVehicleUserDefinedField_ColApplicableVehicleType;

	static const int TblVehicleUserDefinedFieldValue_ColVehicleID;
	static const int TblVehicleUserDefinedFieldValue_ColUserDefinedFieldName;
	static const int TblVehicleUserDefinedFieldValue_ColValue;

	template<typename K, typename V>
	using map = std::map<K, V>;

	template<typename K, typename V>
	using umap = std::unordered_map<K, V>;

	typedef std::string string;
	typedef std::basic_string <unsigned char> ustring;

	typedef sqlite3_stmt* SQLitePreparedStatementPtr;
	
	sqlite3* sqlite;
	umap<string, map<int, int>> sqlQueryBindIndices;
	umap<string, map<int, int>> sqlQueryResultColumnIndices;
	umap<string, SQLitePreparedStatementPtr> sqlQueryPreparedStatements;

	bool Setup(const char* databaseName, std::string& errorMessage);
};

}

#endif
