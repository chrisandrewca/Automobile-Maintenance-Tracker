cmake_minimum_required (VERSION 2.6)

project (AutomobileMaintenanceTracker)

include_directories(include/)

add_executable(AutomobileMaintenanceTracker
	src/cpp/main.cpp
	include/AutomobileMaintenanceTracker.hpp
	src/cpp/utils/Debug.h
	src/cpp/API.cpp
	src/cpp/DataStore.hpp
	src/cpp/DataStore.cpp
	src/cpp/Database.hpp
	src/cpp/Database.cpp

	src/cpp/sqlite/sqlite3.h
	src/cpp/sqlite/sqlite3ext.h
	src/cpp/sqlite/sqlite3.c)