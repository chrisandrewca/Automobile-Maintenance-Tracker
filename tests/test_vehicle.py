import AutomobileMaintenanceTracker as AMT

vehicle = AMT.Vehicle()
assert vehicle.GetID() == -1

vehicle.SetID(19)
assert vehicle.GetID() == 19

vehicle.SetYear(2004)
assert vehicle.GetYear() == 2004

vehicle.SetType("Gasoline")
assert vehicle.GetType() == "Gasoline"

vehicle.SetMake("Volkswagon")
assert vehicle.GetMake() == "Volkswagon"

vehicle.SetModel("Jetta")
assert vehicle.GetModel() == "Jetta"

assert len(vehicle.GetPropertyNames()) == 0

vehicle.SetProperty("Color", "Black")

assert len(vehicle.GetPropertyNames()) == 1

assert vehicle.GetPropertyNames()[0] == "Color"

assert vehicle.GetProperty("Color") == "Black"