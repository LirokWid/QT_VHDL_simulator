/**
 * @file sc_reg.h
 * @brief SystemC module that implements a register and a counter.
 * @details This file contains the definition of the SystemC modules SyscReg and SyscCnt.
 * The SyscReg module implements a register with a single data input and output.
 * The register is synchronous, i.e., the output is updated on the rising edge of the clock signal.
 * The SyscCnt module implements a counter with a single data output.
 *
 * Both modules are implemented as template classes that can be instantiated with the following parameters:
 * - W: width of the data bus (default is 1)
 * - CLKEDGE: boolean value that specifies the clock edge to use (true for rising edge, false for falling edge). Default value is true.
 *
 * The SyscReg and SyscCnt modules are derived from the SyscFlexInt module, which provides a flexible interface for the reset and enable signals of a module.
 * The reset and enable signals can be active high or active low, and can be synchronous or asynchronous.
 * The module can be configured to use the reset and enable signals or not, and can be configured to use the rising edge or falling edge of the clock signal.
 */
#ifndef SC_REG_H
#define SC_REG_H

#include <systemc.h>
#include "sc_flexint.h"


/**
 * @brief The SyscReg class
 * @details A generic SystemC module that implements a register with a single data input and output.
 * The register is synchronous, i.e., the output is updated on the rising edge of the clock signal.
 */
template <unsigned int W = 1, bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>
class SyscReg : public SyscFlexInt<CLKEDGE, FLEXINT>
{
public:
    typedef  sc_lv<W>       data_t;

    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<rst_t>      reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<ena_t>      enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_in<data_t>  d{"d"};                  // Data input
    sc_out<data_t> q{"q"};                  // Data output

    typedef SyscFlexInt<CLKEDGE, FLEXINT> BASE_MODULE;

    typedef SyscReg<W,CLKEDGE, FLEXINT> SC_CURRENT_USER_MODULE;
    SyscReg(::sc_core::sc_module_name name) : SyscFlexInt<CLKEDGE, FLEXINT>(name)
    {
        SC_THREAD(react);
        this->doSensitive();
    }

    void react ()
    {
        while(1)
        {
            if (this->bResetAction())
            {
                q.write(0);
            }
            else if (this->bEnableAction())
            {
                q.write(d.read());
            }
            wait();
        }
    }
};

/**
 * @brief The SyscCnt class
 * @details A generic SystemC module that implements a counter with a single data output.
 * The counter is synchronous, i.e., the output is updated on the rising edge of the clock signal.
 */
template <unsigned int W = 1, bool CLKEDGE = DEFAULT_CLKEDGE, flexinterface_t FLEXINT = DEFAULT_FLEXINT>
class SyscCnt : public SyscFlexInt<CLKEDGE, FLEXINT>
{
public:
    typedef  sc_lv<W>       data_t;
    // ---------------------      Ports      ---------------------
    // Provided by base module scFlexModuleT
    // sc_in_clk         clock{"clock"};    // Clock input (FLEXINT: custom active edge)
    // sc_in<rst_t>      reset{"reset"};    // Asynchronous reset input (FLEXINT: possibly unmapped, custom active level)
    // sc_in<ena_t>      enable{"enable"};  // Synchronous enable input (FLEXINT: possibly unmapped, custom active level)
    sc_out<data_t> q{"q"};                  // Data output

    typedef SyscFlexInt<CLKEDGE, FLEXINT> BASE_MODULE;
    typedef SyscCnt<W,CLKEDGE, FLEXINT> SC_CURRENT_USER_MODULE;
    SyscCnt(::sc_core::sc_module_name name) : SyscFlexInt<CLKEDGE, FLEXINT>(name)
    {
        SC_THREAD(react);
        this->doSensitive();
    }

    void react ()
    {
        while(1)
        {
            if (this->bResetAction())
            {
                q.write(0);
            } else if (this->bEnableAction())
            {
                q.write((sc_uint<W>)q.read() + 1);
            }
            wait();
        }
    }
};


#endif // SC_REG_H
