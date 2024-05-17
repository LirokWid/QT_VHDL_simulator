#ifndef SC_DEBUG_OBJECT_H
#define SC_DEBUG_OBJECT_H

#include <string>
#include <systemc.h>



class debug_object
{
public:
    debug_object(sc_object *o);
    std::string report_value();
    bool isModule();
    bool isAdaptor();
    bool isPort();
    bool isSignal();
    bool isHierarchy();
    bool isTraceable();
    std::string name();

private:
    typedef enum obj_kind_e
    {
        OBJ_KIND_ADAPTOR = 0,
        OBJ_KIND_MODULE,
        OBJ_KIND_CLOCK,
        OBJ_KIND_PORT,
        OBJ_KIND_SIGNAL,
        OBJ_KIND_OTHER
    }obj_kind_t;

    sc_object *obj;
    obj_kind_t sc_kind;

};

#endif // SC_DEBUG_OBJECT_H
