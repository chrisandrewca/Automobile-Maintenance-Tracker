%module AutomobileMaintenanceTracker

%include typemaps.i
%include <std_vector.i>
%include <std_shared_ptr.i>

%{
#include "AutomobileMaintenanceTracker.hpp"
%}

%inline {
	namespace AMT {
		typedef std::string utf8string;
	}
}

%template(VehiclePtr) std::shared_ptr<AMT::Vehicle>;
%template(VehiclePtrVec) std::vector<std::shared_ptr<AMT::Vehicle>>;
%template(VehiclePtrVecPtr) std::shared_ptr<std::vector<std::shared_ptr<AMT::Vehicle>>>;

%template(MaintenanceTaskPtr) std::shared_ptr<AMT::MaintenanceTask>;
%template(MaintenanceTaskPtrVecPtr) std::shared_ptr<std::vector<std::shared_ptr<AMT::MaintenanceTask>>>;

%template(Utf8VecPtr) std::shared_ptr<std::vector<AMT::utf8string>>;

%shared_ptr(AMT::Vehicle);
%shared_ptr(std::vector<AMT::Vehicle>);
%shared_ptr(std::vector<std::shared_ptr<AMT::Vehicle>>);

%shared_ptr(AMT::MaintenanceTask);
%shared_ptr(std::vector<std::shared_ptr<AMT::MaintenanceTask>>);

%shared_ptr(std::vector<AMT::utf8string>);

%typemap(out) int& {
	$result = PyInt_FromLong(*$1);
}

%typemap(out) std::shared_ptr<std::vector<std::shared_ptr<AMT::Vehicle>>> {
	$result = PyList_New((*$1).size());

	unsigned int i = 0;
	for (auto& vehicle_sp : (*$1))
	{
		auto* vehicle_sp_ptr = new std::shared_ptr<AMT::Vehicle>(vehicle_sp);
		PyObject* o = SWIG_NewPointerObj(SWIG_as_voidptr(vehicle_sp_ptr),
						SWIGTYPE_p_std__shared_ptrT_AMT__Vehicle_t, SWIG_POINTER_OWN);
		PyList_SetItem($result, i, o);
		i++;
	}
}

%include "AutomobileMaintenanceTracker.hpp"