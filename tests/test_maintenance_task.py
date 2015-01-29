import AutomobileMaintenanceTracker as AMT

task = AMT.MaintenanceTask()
assert task.GetID() == -1
assert task.VehicleID() == -1

task.SetID(24)
assert task.GetID() == 24

task.SetVehicleID(42)
assert task.VehicleID() == 42

task.SetType("Oil Change")
assert task.GetType() == "Oil Change"

task.SetDate(1234)
assert task.GetDate() == 1234