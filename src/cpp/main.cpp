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
	return 0;
}