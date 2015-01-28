%module AutomobileMaintenanceTracker

%{
#include "AutomobileMaintenanceTracker.hpp"
%}

%include <std_vector.i>
%include <std_shared_ptr.i>

%template(VehiclePtr) std::shared_ptr<AMT::Vehicle>;
%template(MaintenanceTaskPtr) std::shared_ptr<AMT::MaintenanceTask>;
%template(Utf8VecPtr) std::shared_ptr<std::vector<AMT::utf8string>>;
%template(VehiclePtrVecPtr) std::shared_ptr<std::vector<std::shared_ptr<AMT::Vehicle>>>;
%template(MaintenanceTaskPtrVecPtr) std::shared_ptr<std::vector<std::shared_ptr<AMT::MaintenanceTask>>>;

%apply int *OUTPUT {int&};

%include "AutomobileMaintenanceTracker.hpp"