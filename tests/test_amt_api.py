import AutomobileMaintenanceTracker as AMT
api = AMT.AMTAPI(AMT.AMTAPI.DataStoreOption_Database)

api.AddTypeOfVehicle("Electric")
vehicleTypes = api.ListAllTypesOfVehicles()
assert "Electric" in vehicleTypes

vehicle = api.CreateVehicle()
assert vehicle.GetID() > -1

vehicle.SetType("Electric")
vehicle.SetMake("Tesla")
vehicle.SetModel("Model S")
vehicle.SetProperty("Color", "Black")
vehicle.SetProperty("Price", "99999.99")
api.UpdateVehicle(vehicle)

response = api.GetVehicle(vehicle.GetID())
assert response.GetType() == "Electric"
assert response.GetMake() == "Tesla"
assert response.GetModel() == "Model S"
assert response.GetProperty("Color") == "Black"
assert response.GetProperty("Price") == "99999.99"

task = api.CreateMaintenanceTask(vehicle.GetID())
assert task.GetID() > -1

task.SetType("Oil Change")
task.SetVehicleID(vehicle.GetID())
task.SetDate(1234)
api.UpdateMaintenanceTask(task)

response = api.ListVehicleMaintenanceHistory(task.VehicleID())
assert len(response) == 1
assert response[0].GetID() == task.GetID()

taskTypes = api.ListAllTypesOfMaintenance()
assert "Oil Change" in taskTypes

vehicle = api.CreateVehicle()
vehicle.SetType("Gasoline")
vehicle.SetMake("Volkswagon")
vehicle.SetModel("Jetta")
vehicle.SetYear(2014)

api.UpdateVehicle(vehicle, AMT.Vehicle.Properties_Type
							| AMT.Vehicle.Properties_Make
							| AMT.Vehicle.Properties_Year)

response = api.GetVehicle(vehicle.GetID())
assert response.GetType() == "Gasoline"
assert response.GetMake() == "Volkswagon"
assert response.GetYear() == 2014
assert not response.GetModel()

vid = vehicle.GetID()
api.DeleteVehicle(vehicle.GetID())
response = api.GetVehicle(vid)
assert response.GetID() == -1

api.DeleteMaintenanceTask(task.GetID())