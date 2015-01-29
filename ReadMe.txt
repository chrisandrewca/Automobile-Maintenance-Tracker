=================================================================
Automobile Maintenance Tracker
=================================================================
-----------------------------------------------------------------
Software Requirements
-----------------------------------------------------------------
C++ 11 compiler
SQLite 3 (included)
Python 2.7
SWIG (included)
CMake
Windows (fully Linux compatibile with CMakeLists.txt path adjustments)

-----------------------------------------------------------------
Build Prep
-----------------------------------------------------------------
Define a couple system environment variables for use by SWIG:
    PYTHON_INCLUDE: C:\python27\include
    PYTHON_LIB: C:\python27\libs\python27.lib

Run CMake:
    cd build
    cmake ../

Open the project and make sure to select "Release" mode

Build!

-----------------------------------------------------------------
Running the test harness
-----------------------------------------------------------------
Open a terminal window:
    cd tests
    python test_amt_api.py (or test_vehicle.py, test_maintenance_task.py)

Handy firefox addon to view SQLite DBs https://addons.mozilla.org/en-US/firefox/addon/sqlite-manager/

-----------------------------------------------------------------
Source Breakdown
-----------------------------------------------------------------
   .git/ - Commit history
  build/ - CMake build dir
codegen/ - SWIG interface (.i) file and generated code
include/ - Single header AutomobileMaintenanceTracker.hpp
    src/ - C++ files
  tests/ - Python test harness for the API
  tools/ - SWIGwin

include/AutomobileMaintenanceTracker.hpp  - API interface / entry
src/AMTAPI.cpp                            - Client facing API implementation
src/Database.hpp                          - Implements the API and stores the data in a SQLite3 database
src/Vehicle.cpp                           - Vehicle data
src/MaintenanceTask.cpp                   - Maintenance Task data
src/AutomobileMaintenanceTracker_wrap.cxx - SWIG generated API wrapper for Python interop