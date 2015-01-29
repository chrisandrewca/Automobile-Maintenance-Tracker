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