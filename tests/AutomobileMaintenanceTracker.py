# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_AutomobileMaintenanceTracker', [dirname(__file__)])
        except ImportError:
            import _AutomobileMaintenanceTracker
            return _AutomobileMaintenanceTracker
        if fp is not None:
            try:
                _mod = imp.load_module('_AutomobileMaintenanceTracker', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _AutomobileMaintenanceTracker = swig_import_helper()
    del swig_import_helper
else:
    import _AutomobileMaintenanceTracker
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0


class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_SwigPyIterator
    __del__ = lambda self: None

    def value(self):
        return _AutomobileMaintenanceTracker.SwigPyIterator_value(self)

    def incr(self, n=1):
        return _AutomobileMaintenanceTracker.SwigPyIterator_incr(self, n)

    def decr(self, n=1):
        return _AutomobileMaintenanceTracker.SwigPyIterator_decr(self, n)

    def distance(self, x):
        return _AutomobileMaintenanceTracker.SwigPyIterator_distance(self, x)

    def equal(self, x):
        return _AutomobileMaintenanceTracker.SwigPyIterator_equal(self, x)

    def copy(self):
        return _AutomobileMaintenanceTracker.SwigPyIterator_copy(self)

    def next(self):
        return _AutomobileMaintenanceTracker.SwigPyIterator_next(self)

    def __next__(self):
        return _AutomobileMaintenanceTracker.SwigPyIterator___next__(self)

    def previous(self):
        return _AutomobileMaintenanceTracker.SwigPyIterator_previous(self)

    def advance(self, n):
        return _AutomobileMaintenanceTracker.SwigPyIterator_advance(self, n)

    def __eq__(self, x):
        return _AutomobileMaintenanceTracker.SwigPyIterator___eq__(self, x)

    def __ne__(self, x):
        return _AutomobileMaintenanceTracker.SwigPyIterator___ne__(self, x)

    def __iadd__(self, n):
        return _AutomobileMaintenanceTracker.SwigPyIterator___iadd__(self, n)

    def __isub__(self, n):
        return _AutomobileMaintenanceTracker.SwigPyIterator___isub__(self, n)

    def __add__(self, n):
        return _AutomobileMaintenanceTracker.SwigPyIterator___add__(self, n)

    def __sub__(self, *args):
        return _AutomobileMaintenanceTracker.SwigPyIterator___sub__(self, *args)
    def __iter__(self):
        return self
SwigPyIterator_swigregister = _AutomobileMaintenanceTracker.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _AutomobileMaintenanceTracker.SHARED_PTR_DISOWN
class Utf8VecPtr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Utf8VecPtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Utf8VecPtr, name)
    __repr__ = _swig_repr

    def __init__(self):
        this = _AutomobileMaintenanceTracker.new_Utf8VecPtr()
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_Utf8VecPtr
    __del__ = lambda self: None
Utf8VecPtr_swigregister = _AutomobileMaintenanceTracker.Utf8VecPtr_swigregister
Utf8VecPtr_swigregister(Utf8VecPtr)

class VehiclePtr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VehiclePtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VehiclePtr, name)
    __repr__ = _swig_repr

    def __init__(self):
        this = _AutomobileMaintenanceTracker.new_VehiclePtr()
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_VehiclePtr
    __del__ = lambda self: None
VehiclePtr_swigregister = _AutomobileMaintenanceTracker.VehiclePtr_swigregister
VehiclePtr_swigregister(VehiclePtr)

class VehiclePtrVec(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VehiclePtrVec, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VehiclePtrVec, name)
    __repr__ = _swig_repr

    def iterator(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_iterator(self)
    def __iter__(self):
        return self.iterator()

    def __nonzero__(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___nonzero__(self)

    def __bool__(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___bool__(self)

    def __len__(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___len__(self)

    def pop(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_pop(self)

    def __getslice__(self, i, j):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___getslice__(self, i, j)

    def __setslice__(self, *args):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___setslice__(self, *args)

    def __delslice__(self, i, j):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___delslice__(self, i, j)

    def __delitem__(self, *args):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___delitem__(self, *args)

    def __getitem__(self, *args):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___getitem__(self, *args)

    def __setitem__(self, *args):
        return _AutomobileMaintenanceTracker.VehiclePtrVec___setitem__(self, *args)

    def append(self, x):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_append(self, x)

    def empty(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_empty(self)

    def size(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_size(self)

    def clear(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_clear(self)

    def swap(self, v):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_swap(self, v)

    def get_allocator(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_get_allocator(self)

    def begin(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_begin(self)

    def end(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_end(self)

    def rbegin(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_rbegin(self)

    def rend(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_rend(self)

    def pop_back(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_pop_back(self)

    def erase(self, *args):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_erase(self, *args)

    def __init__(self, *args):
        this = _AutomobileMaintenanceTracker.new_VehiclePtrVec(*args)
        try:
            self.this.append(this)
        except:
            self.this = this

    def push_back(self, x):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_push_back(self, x)

    def front(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_front(self)

    def back(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_back(self)

    def assign(self, n, x):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_assign(self, n, x)

    def resize(self, *args):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_resize(self, *args)

    def insert(self, *args):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_insert(self, *args)

    def reserve(self, n):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_reserve(self, n)

    def capacity(self):
        return _AutomobileMaintenanceTracker.VehiclePtrVec_capacity(self)
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_VehiclePtrVec
    __del__ = lambda self: None
VehiclePtrVec_swigregister = _AutomobileMaintenanceTracker.VehiclePtrVec_swigregister
VehiclePtrVec_swigregister(VehiclePtrVec)

class VehiclePtrVecPtr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VehiclePtrVecPtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VehiclePtrVecPtr, name)
    __repr__ = _swig_repr

    def __init__(self):
        this = _AutomobileMaintenanceTracker.new_VehiclePtrVecPtr()
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_VehiclePtrVecPtr
    __del__ = lambda self: None
VehiclePtrVecPtr_swigregister = _AutomobileMaintenanceTracker.VehiclePtrVecPtr_swigregister
VehiclePtrVecPtr_swigregister(VehiclePtrVecPtr)

class MaintenanceTaskPtr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, MaintenanceTaskPtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, MaintenanceTaskPtr, name)
    __repr__ = _swig_repr

    def __init__(self):
        this = _AutomobileMaintenanceTracker.new_MaintenanceTaskPtr()
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_MaintenanceTaskPtr
    __del__ = lambda self: None
MaintenanceTaskPtr_swigregister = _AutomobileMaintenanceTracker.MaintenanceTaskPtr_swigregister
MaintenanceTaskPtr_swigregister(MaintenanceTaskPtr)

class MaintenanceTaskPtrVecPtr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, MaintenanceTaskPtrVecPtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, MaintenanceTaskPtrVecPtr, name)
    __repr__ = _swig_repr

    def __init__(self):
        this = _AutomobileMaintenanceTracker.new_MaintenanceTaskPtrVecPtr()
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_MaintenanceTaskPtrVecPtr
    __del__ = lambda self: None
MaintenanceTaskPtrVecPtr_swigregister = _AutomobileMaintenanceTracker.MaintenanceTaskPtrVecPtr_swigregister
MaintenanceTaskPtrVecPtr_swigregister(MaintenanceTaskPtrVecPtr)

class Vehicle(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vehicle, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vehicle, name)
    __repr__ = _swig_repr
    Properties_All = _AutomobileMaintenanceTracker.Vehicle_Properties_All
    Properties_Type = _AutomobileMaintenanceTracker.Vehicle_Properties_Type
    Properties_Make = _AutomobileMaintenanceTracker.Vehicle_Properties_Make
    Properties_Model = _AutomobileMaintenanceTracker.Vehicle_Properties_Model
    Properties_Year = _AutomobileMaintenanceTracker.Vehicle_Properties_Year
    Properties_Odometer = _AutomobileMaintenanceTracker.Vehicle_Properties_Odometer
    Properties_UserDefined = _AutomobileMaintenanceTracker.Vehicle_Properties_UserDefined

    def __init__(self, ID=-1):
        this = _AutomobileMaintenanceTracker.new_Vehicle(ID)
        try:
            self.this.append(this)
        except:
            self.this = this

    def GetID(self):
        return _AutomobileMaintenanceTracker.Vehicle_GetID(self)

    def SetID(self, ID):
        return _AutomobileMaintenanceTracker.Vehicle_SetID(self, ID)

    def GetYear(self):
        return _AutomobileMaintenanceTracker.Vehicle_GetYear(self)

    def SetYear(self, year):
        return _AutomobileMaintenanceTracker.Vehicle_SetYear(self, year)

    def GetOdometer(self):
        return _AutomobileMaintenanceTracker.Vehicle_GetOdometer(self)

    def SetOdometer(self, value):
        return _AutomobileMaintenanceTracker.Vehicle_SetOdometer(self, value)

    def GetType(self):
        return _AutomobileMaintenanceTracker.Vehicle_GetType(self)

    def SetType(self, type):
        return _AutomobileMaintenanceTracker.Vehicle_SetType(self, type)

    def GetMake(self):
        return _AutomobileMaintenanceTracker.Vehicle_GetMake(self)

    def SetMake(self, make):
        return _AutomobileMaintenanceTracker.Vehicle_SetMake(self, make)

    def GetModel(self):
        return _AutomobileMaintenanceTracker.Vehicle_GetModel(self)

    def SetModel(self, model):
        return _AutomobileMaintenanceTracker.Vehicle_SetModel(self, model)

    def GetPropertyNames(self):
        return _AutomobileMaintenanceTracker.Vehicle_GetPropertyNames(self)

    def GetProperty(self, name):
        return _AutomobileMaintenanceTracker.Vehicle_GetProperty(self, name)

    def SetProperty(self, property, value):
        return _AutomobileMaintenanceTracker.Vehicle_SetProperty(self, property, value)
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_Vehicle
    __del__ = lambda self: None
Vehicle_swigregister = _AutomobileMaintenanceTracker.Vehicle_swigregister
Vehicle_swigregister(Vehicle)

class MaintenanceTask(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, MaintenanceTask, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, MaintenanceTask, name)
    __repr__ = _swig_repr
    Properties_All = _AutomobileMaintenanceTracker.MaintenanceTask_Properties_All
    Properties_Type = _AutomobileMaintenanceTracker.MaintenanceTask_Properties_Type
    Properties_Date = _AutomobileMaintenanceTracker.MaintenanceTask_Properties_Date
    Properties_VehicleID = _AutomobileMaintenanceTracker.MaintenanceTask_Properties_VehicleID

    def __init__(self, ID=-1, vehicleID=-1):
        this = _AutomobileMaintenanceTracker.new_MaintenanceTask(ID, vehicleID)
        try:
            self.this.append(this)
        except:
            self.this = this

    def GetID(self):
        return _AutomobileMaintenanceTracker.MaintenanceTask_GetID(self)

    def SetID(self, ID):
        return _AutomobileMaintenanceTracker.MaintenanceTask_SetID(self, ID)

    def VehicleID(self):
        return _AutomobileMaintenanceTracker.MaintenanceTask_VehicleID(self)

    def SetVehicleID(self, ID):
        return _AutomobileMaintenanceTracker.MaintenanceTask_SetVehicleID(self, ID)

    def GetType(self):
        return _AutomobileMaintenanceTracker.MaintenanceTask_GetType(self)

    def SetType(self, type):
        return _AutomobileMaintenanceTracker.MaintenanceTask_SetType(self, type)

    def GetDate(self):
        return _AutomobileMaintenanceTracker.MaintenanceTask_GetDate(self)

    def SetDate(self, date):
        return _AutomobileMaintenanceTracker.MaintenanceTask_SetDate(self, date)
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_MaintenanceTask
    __del__ = lambda self: None
MaintenanceTask_swigregister = _AutomobileMaintenanceTracker.MaintenanceTask_swigregister
MaintenanceTask_swigregister(MaintenanceTask)

class APIBase(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, APIBase, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, APIBase, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_APIBase
    __del__ = lambda self: None

    def AddTypeOfVehicle(self, name):
        return _AutomobileMaintenanceTracker.APIBase_AddTypeOfVehicle(self, name)

    def UpdateTypesOfVehicles(self, name, newName):
        return _AutomobileMaintenanceTracker.APIBase_UpdateTypesOfVehicles(self, name, newName)

    def ListAllTypesOfVehicles(self):
        return _AutomobileMaintenanceTracker.APIBase_ListAllTypesOfVehicles(self)

    def CreateVehicle(self):
        return _AutomobileMaintenanceTracker.APIBase_CreateVehicle(self)

    def DeleteVehicle(self, vehicleID):
        return _AutomobileMaintenanceTracker.APIBase_DeleteVehicle(self, vehicleID)

    def ListAllVehicles(self):
        return _AutomobileMaintenanceTracker.APIBase_ListAllVehicles(self)

    def FindVehicles(self, properties, values):
        return _AutomobileMaintenanceTracker.APIBase_FindVehicles(self, properties, values)

    def GetVehicle(self, vehicleID):
        return _AutomobileMaintenanceTracker.APIBase_GetVehicle(self, vehicleID)

    def UpdateVehicle(self, *args):
        return _AutomobileMaintenanceTracker.APIBase_UpdateVehicle(self, *args)

    def AddTypeOfMaintenance(self, name):
        return _AutomobileMaintenanceTracker.APIBase_AddTypeOfMaintenance(self, name)

    def UpdateTypesOfMaintenance(self, type, newType):
        return _AutomobileMaintenanceTracker.APIBase_UpdateTypesOfMaintenance(self, type, newType)

    def ListAllTypesOfMaintenance(self):
        return _AutomobileMaintenanceTracker.APIBase_ListAllTypesOfMaintenance(self)

    def CreateMaintenanceTask(self, vehicleID):
        return _AutomobileMaintenanceTracker.APIBase_CreateMaintenanceTask(self, vehicleID)

    def UpdateMaintenanceTask(self, *args):
        return _AutomobileMaintenanceTracker.APIBase_UpdateMaintenanceTask(self, *args)

    def DeleteMaintenanceTask(self, taskID):
        return _AutomobileMaintenanceTracker.APIBase_DeleteMaintenanceTask(self, taskID)

    def ListVehicleMaintenanceHistory(self, *args):
        return _AutomobileMaintenanceTracker.APIBase_ListVehicleMaintenanceHistory(self, *args)
APIBase_swigregister = _AutomobileMaintenanceTracker.APIBase_swigregister
APIBase_swigregister(APIBase)

class AMTAPI(APIBase):
    __swig_setmethods__ = {}
    for _s in [APIBase]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, AMTAPI, name, value)
    __swig_getmethods__ = {}
    for _s in [APIBase]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, AMTAPI, name)
    __repr__ = _swig_repr
    DataStoreOption_Database = _AutomobileMaintenanceTracker.AMTAPI_DataStoreOption_Database

    def __init__(self, dataStore):
        this = _AutomobileMaintenanceTracker.new_AMTAPI(dataStore)
        try:
            self.this.append(this)
        except:
            self.this = this
    __swig_destroy__ = _AutomobileMaintenanceTracker.delete_AMTAPI
    __del__ = lambda self: None

    def AddTypeOfVehicle(self, name):
        return _AutomobileMaintenanceTracker.AMTAPI_AddTypeOfVehicle(self, name)

    def UpdateTypesOfVehicles(self, name, newName):
        return _AutomobileMaintenanceTracker.AMTAPI_UpdateTypesOfVehicles(self, name, newName)

    def ListAllTypesOfVehicles(self):
        return _AutomobileMaintenanceTracker.AMTAPI_ListAllTypesOfVehicles(self)

    def CreateVehicle(self):
        return _AutomobileMaintenanceTracker.AMTAPI_CreateVehicle(self)

    def DeleteVehicle(self, vehicleID):
        return _AutomobileMaintenanceTracker.AMTAPI_DeleteVehicle(self, vehicleID)

    def ListAllVehicles(self):
        return _AutomobileMaintenanceTracker.AMTAPI_ListAllVehicles(self)

    def FindVehicles(self, properties, values):
        return _AutomobileMaintenanceTracker.AMTAPI_FindVehicles(self, properties, values)

    def GetVehicle(self, vehicleID):
        return _AutomobileMaintenanceTracker.AMTAPI_GetVehicle(self, vehicleID)

    def UpdateVehicle(self, *args):
        return _AutomobileMaintenanceTracker.AMTAPI_UpdateVehicle(self, *args)

    def AddTypeOfMaintenance(self, name):
        return _AutomobileMaintenanceTracker.AMTAPI_AddTypeOfMaintenance(self, name)

    def UpdateTypesOfMaintenance(self, type, newType):
        return _AutomobileMaintenanceTracker.AMTAPI_UpdateTypesOfMaintenance(self, type, newType)

    def ListAllTypesOfMaintenance(self):
        return _AutomobileMaintenanceTracker.AMTAPI_ListAllTypesOfMaintenance(self)

    def CreateMaintenanceTask(self, vehicleID):
        return _AutomobileMaintenanceTracker.AMTAPI_CreateMaintenanceTask(self, vehicleID)

    def UpdateMaintenanceTask(self, *args):
        return _AutomobileMaintenanceTracker.AMTAPI_UpdateMaintenanceTask(self, *args)

    def DeleteMaintenanceTask(self, taskID):
        return _AutomobileMaintenanceTracker.AMTAPI_DeleteMaintenanceTask(self, taskID)

    def ListVehicleMaintenanceHistory(self, *args):
        return _AutomobileMaintenanceTracker.AMTAPI_ListVehicleMaintenanceHistory(self, *args)
AMTAPI_swigregister = _AutomobileMaintenanceTracker.AMTAPI_swigregister
AMTAPI_swigregister(AMTAPI)

# This file is compatible with both classic and new-style classes.


