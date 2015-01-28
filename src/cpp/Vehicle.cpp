#include "AutomobileMaintenanceTracker.hpp"
using namespace AMT;

Vehicle::Vehicle(int ID) :
    id(ID),
    year(0),
    odometer(0),
    type(),
    make(),
    model(),
    properties(),
    propertyNames()
{
}

int& Vehicle::GetID()
{
    return this->id;
}

void Vehicle::SetID(int ID)
{
	this->id = ID;
}

int& Vehicle::GetYear()
{
    return this->year;
}

void Vehicle::SetYear(int year)
{
	this->year = year;
}

int& Vehicle::GetOdometer()
{
    return this->odometer;
}

void Vehicle::SetOdometer(int value)
{
	this->odometer = value;
}

utf8string& Vehicle::GetType()
{
    return this->type;
}

void Vehicle::SetType(const utf8string& type)
{
	this->type = type;
}

utf8string& Vehicle::GetMake()
{
    return this->make;
}

void Vehicle::SetMake(const utf8string& make)
{
	this->make = make;
}

utf8string& Vehicle::GetModel()
{
    return this->model;
}

void Vehicle::SetModel(const utf8string& model)
{
	this->model = model;
}

std::vector<utf8string>& Vehicle::GetPropertyNames()
{
    return this->propertyNames;
}

utf8string& Vehicle::GetProperty(const utf8string& name)
{
    return this->properties[name];
}

void Vehicle::SetProperty(const utf8string& property, const utf8string& value)
{
    if (this->properties.find(property) == this->properties.end())
        this->propertyNames.push_back(property);

    this->properties[property] = value;
}
