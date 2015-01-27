#include "AutomobileMaintenanceTracker.hpp"
using namespace AMT;

Vehicle::Vehicle(int id) :
    id(id),
    year(0),
    odometer(0),
    type(),
    make(),
    model(),
    properties()
{
}

int& Vehicle::GetID()
{
    return this->id;
}

int& Vehicle::GetYear()
{
    return this->year;
}

int& Vehicle::GetOdometer()
{
    return this->odometer;
}

utf8string& Vehicle::GetType()
{
    return this->type;
}

utf8string& Vehicle::GetMake()
{
    return this->make;
}

utf8string& Vehicle::GetModel()
{
    return this->model;
}
