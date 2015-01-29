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
Define a couple system environment variables:
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