cmake_minimum_required (VERSION 2.6)

project (AutomobileMaintenanceTracker C CXX)

if (${UNIX})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++11")
endif (${UNIX})

include_directories(include/)

add_executable(AutomobileMaintenanceTracker
	src/cpp/main.cpp
	include/AutomobileMaintenanceTracker.hpp
	src/cpp/utils/Debug.h
	src/cpp/AMTAPI.cpp
	src/cpp/Database.hpp
	src/cpp/Database.cpp
	src/cpp/Vehicle.cpp
	src/cpp/MaintenanceTask.cpp

	src/cpp/sqlite/sqlite3.h
	src/cpp/sqlite/sqlite3ext.h
	src/cpp/sqlite/sqlite3.c)

if (${UNIX})
	find_package (Threads)
	target_link_libraries (AutomobileMaintenanceTracker ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
endif (${UNIX})