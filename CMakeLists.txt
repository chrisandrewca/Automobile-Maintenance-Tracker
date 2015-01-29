cmake_minimum_required (VERSION 2.6)

project (AutomobileMaintenanceTracker C CXX)

set(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

if (${UNIX})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++11")
endif (${UNIX})

IF (WIN32)
#	include_directories(C:/Python27/include)
#	list(APPEND CMAKE_LIBRARY_PATH C:/Python27/libs)
	execute_process(COMMAND "${CMAKE_CURRENT_LIST_DIR}/tools/swigwin/swig.exe" -c++ -python -I${CMAKE_CURRENT_LIST_DIR}/include -o ${CMAKE_CURRENT_LIST_DIR}/codegen/AutomobileMaintenanceTracker_wrap.cxx ${CMAKE_CURRENT_LIST_DIR}/codegen/AutomobileMaintenanceTracker.i)
ENDIF()

include_directories(include/)

#add_executable(AutomobileMaintenanceTracker
add_library(AutomobileMaintenanceTracker SHARED
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
	src/cpp/sqlite/sqlite3.c

	codegen/AutomobileMaintenanceTracker_wrap.cxx)

if (${UNIX})
	find_package (Threads)
	target_link_libraries (AutomobileMaintenanceTracker ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
endif (${UNIX})

IF (WIN32)
	target_link_libraries (AutomobileMaintenanceTracker ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} ${PYTHON_LIBRARIES})
ENDIF()

set_target_properties(AutomobileMaintenanceTracker PROPERTIES OUTPUT_NAME "_AutomobileMaintenanceTracker")
set_target_properties(AutomobileMaintenanceTracker PROPERTIES SUFFIX ".pyd")

add_custom_command(TARGET AutomobileMaintenanceTracker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/build/${CMAKE_BUILD_TYPE}/_AutomobileMaintenanceTracker.pyd ${CMAKE_CURRENT_LIST_DIR}/tests
)

add_custom_command(TARGET AutomobileMaintenanceTracker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/codegen/AutomobileMaintenanceTracker.py ${CMAKE_CURRENT_LIST_DIR}/tests
)