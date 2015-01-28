#include "Database.hpp"
#include <iostream>
#include <string>
using namespace AMT;

//int main(int argc, char* argv[])
//{
//    AMTAPI api(AMTAPI::DataStoreOption::Database);
//    api.AddTypeOfVehicle("Electric");
//    api.UpdateTypesOfVehicles("Electric", "Diesel");
//    api.UpdateTypesOfVehicles("Diesel", "Gasoline");
//    api.UpdateTypesOfVehicles("Gasoline", "Electric2");
//    api.AddTypeOfVehicle("Diesel");
//    api.AddTypeOfVehicle("Gasoline");
//
//    auto vehicleTypes = api.ListAllTypesOfVehicles();
//    for (std::size_t i = 0; i < vehicleTypes->size(); i++)
//    {
//        std::cout << vehicleTypes->at(i) << "\n";
//    }
//
//    auto vehicle = api.CreateVehicle();
//    api.CreateVehicle();
//    api.CreateVehicle();
//    api.CreateVehicle();
//    auto v2 = api.CreateVehicle();
//
//    api.DeleteVehicle(vehicle->GetID());
//
//    v2->GetType() = "Diesel";
//    //v2->GetMake() = "Dodge";
//    v2->GetModel() = "DodgeTruck";
//    api.UpdateVehicle(*v2);
//
//    auto vehicles = api.ListAllVehicles();
//    for (auto& v : *vehicles)
//    {
//        std::cout << "v id: " << v->GetID() << "\n";
//        std::cout << "v type " << v->GetType() << "\n";
//        std::cout << "v make: " << v->GetMake() << "\n";
//        std::cout << "v model: " << v->GetModel() << "\n";
//    }
//
//	return 0;
//}
