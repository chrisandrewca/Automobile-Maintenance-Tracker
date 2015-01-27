#include "Database.hpp"
#include <iostream>
#include <string>
using namespace AMT;

int main(int argc, char* argv[])
{
	const char* DB_NAME = "auto_maintenance_tracker";
	Database db;
	std::string errMsg;
	db.Open(DB_NAME, errMsg);
	std::cout << errMsg << std::endl;

	db.AddTypeOfVehicle("Electric");
	db.UpdateTypesOfVehicles("Electric", "Diesel");
	db.UpdateTypesOfVehicles("Diesel", "Gasoline");
	db.UpdateTypesOfVehicles("Gasoline", "Electric2");
	db.AddTypeOfVehicle("Diesel");
	db.AddTypeOfVehicle("Gasoline");
	auto vehicleTypes = db.ListAllTypesOfVehicles();
    for (std::size_t i = 0; i < vehicleTypes->size(); i++)
	{
		std::cout << vehicleTypes->at(i) << "\n";
	}

    auto vehicle = db.CreateVehicle();
    db.CreateVehicle();
    db.CreateVehicle();
    db.CreateVehicle();
    db.CreateVehicle();

    db.DeleteVehicle(*vehicle);

    auto vehicles = db.ListAllVehicles();
    for (auto& v : *vehicles)
    {
        std::cout << "v id: " << v->GetID() << "\n";
    }

	return 0;
}
