#ifndef SC_T_GATES_H
#define SC_T_GATES_H

#include <systemc.h>
#include <stdio.h>

#undef  ABORT_IF_NULL_PTR
#define ABORT_IF_NULL_PTR(ptr)  \
    if (ptr == nullptr) {\
        char buffer[1024];\
        snprintf(buffer, sizeof(buffer), "%s:%d", __FILE__, __LINE__);\
        SC_REPORT_ERROR(buffer, "Memory allocation failed");\
        sc_core::sc_abort(); \
    }

#define ABORT_CAUSE(msg)  \
    {\
        char buffer[1024];\
        snprintf(buffer, sizeof(buffer), "%s:%d", __FILE__, __LINE__);\
        SC_REPORT_ERROR(buffer, msg);\
        sc_core::sc_abort(); \
    }

/**
 * @brief The SyscTComb::sc_core::sc_module template class
 * @details A generic SystemC template module that implements a combinational block with N inputs
 * of Win bits each (called d0, d1, etc.) and a single Wout-bits output called y.
 * The modeled circuit is combinational, i.e., the output is updated whenever any of the inputs changes.
 */
template<unsigned int W = 1>
class SyscTComb : public ::sc_core::sc_module
{
public:
    static_assert(W >= 1, "SyscTComb DESIGN ERROR: W must be >= 1");
    static_assert(W <= 4, "SyscTComb DESIGN ERROR: W must be <= 4");

    typedef  sc_lv<W>       data_t;

    /**
     * Data output
     * Single output port initialized to the name "y"
     */
    sc_out<data_t> y{"y"};

    /**
     * Data input(s)
     * Array of pointers so each port name can be initialized in the constructor to 'd0', 'd1', and so on (instead of 'port_0', 'port_1', etc.)
     * mapping is done as (*mux_object.d[index])(signal_to_bind_the_port) instead of mux_object.d[index](signal_to_bind_the_port)
     * see https://stackoverflow.com/questions/35425052/how-to-initialize-a-systemc-port-name-which-is-an-array/35535730#35535730
     */
    sc_in<data_t> **d;

public:

    typedef SyscTComb<W> SC_CURRENT_USER_MODULE;
    SyscTComb(const char *name, unsigned int N = 2)
        : ::sc_core::sc_module(name), m_N(N)
    {
        SC_METHOD(combinational);
        if (N < 2) {
            ABORT_CAUSE("Combinational circuit with N < 2");
        }
        d = new sc_in<data_t> *[N];
        ABORT_IF_NULL_PTR(d);
        for(uint32_t i=0; i < N; i++)
        {
            d[i] = new sc_in<data_t>(("d" + std::to_string(i)).c_str());
            ABORT_IF_NULL_PTR(d[i]);
            sensitive << *d[i];
        }
    }

    ~SyscTComb()
    {
        if (d != nullptr) {
            for(uint32_t i=0; i < m_N; i++)
            {
                if (d[i] != nullptr) {
                    delete d[i];
                }
            }
            delete d;
        }
    }

    virtual void combinational()
    {
        data_t result(SC_LOGIC_X);
        y.write(result);
    }

protected:
    unsigned int m_N;
};


template<unsigned int W = 1>
class SyscTAnd : public SyscTComb<W>
{
public:

    typedef SyscTComb<W> BASE_MODULE;
    typedef SyscTAnd<W>  SC_CURRENT_USER_MODULE;
    SyscTAnd(const char *name, unsigned int N = 2)
        : SyscTComb<W>(name, N)
    {
    }

    virtual void combinational()
    {
        auto result = BASE_MODULE::d[0]->read();
        for (unsigned int i = 1; i < BASE_MODULE::m_N; i++) {
            result &= BASE_MODULE::d[i]->read();
        }
        BASE_MODULE::y.write(result);
    }
};

#endif // SC_T_GATES_H
