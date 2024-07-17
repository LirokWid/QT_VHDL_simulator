#include "systemcbuilder.h"
#include "systemC/systemc_modules/sc_gates.h"
#include "systemC/systemc_modules/sc_mux.h"
#include "systemC/systemc_modules/sc_terminals.h"

SystemCBuilder::SystemCBuilder(s_components_list *components) :
    components(components)
{
    if (components->isEmpty())
    {
        debug->addError("no components found, systemC builder abort");
        return;
    }
    else
    {
        buildSystemCSim(*components);
    }

}

/*  Trying to use templates to create modules // Not working
// Function to create SystemC module
template <unsigned int N, unsigned int W, unsigned int SELW>
SyscMux<N, W, SELW>* SystemCBuilder::createMuxModule(const QString& name)
{
    return new SyscMux<N, W, SELW>(name.toStdString().c_str());
}

template <template <unsigned int, unsigned int> class GateType, unsigned int N, unsigned int W>
GateType<N, W>* SystemCBuilder::createGateModule(const QString& name)
{
    return new GateType<N, W>(name.toStdString().c_str());
}
*/

void SystemCBuilder::buildSystemCSim(const s_components_list& components)
{
    // Elements module creation
    for (const s_element &elem : components.elements.elements_list)
    {
        sc_module* module = createModule(deviceFinder(elem.device), elem);
        if (module != nullptr)
        {
            modules.push_back(module);
        }
    }

    // IOs creation TODO
    for (const s_sim_I_O &io : components.simulation_IOs.i_os)
    {
        if(io.type == "input")
        {
            // 1.Find which module to use
            sc_module* module = createSysC_IO(io);
        }
        else if(io.type == "output")
        {
            sc_module* module = createSysC_IO(io);
        }
        else
        {
            debug->addError("IO type not recognized");
        }




        if (module != nullptr)
        {
            modules.push_back(module);
        }
    }
}

device SystemCBuilder::deviceFinder(QString deviceName)
{
    // Predefined lists of device names
    // TODO : has to be update to every device names
    const QStringList multiplexerDevices    = {"mux2_1", "mux4_1"};
    const QStringList andGateDevices        = {"and2", "and4"};
    const QStringList orGateDevices         = {"or2", "or4"};
    const QStringList nandGateDevices       = {"nand2", "nand4"};
    const QStringList norGateDevices        = {"nor2", "nor4"};
    const QStringList xorGateDevices        = {"xor2", "xor4"};
    const QStringList xnorGateDevices       = {"xnor2", "xnor4"};

    if (multiplexerDevices.contains(deviceName))
    {
        return MULTIPLEXER_DEVICE;
    }
    else if (andGateDevices.contains(deviceName))
    {
        return AND_GATE_DEVICE;
    }
    else if (orGateDevices.contains(deviceName))
    {
        return OR_GATE_DEVICE;
    }
    else if (nandGateDevices.contains(deviceName))
    {
        return NAND_GATE_DEVICE;
    }
    else if (norGateDevices.contains(deviceName))
    {
        return NOR_GATE_DEVICE;
    }
    else if (xorGateDevices.contains(deviceName))
    {
        return XOR_GATE_DEVICE;
    }
    else if (xnorGateDevices.contains(deviceName))
    {
        return XNOR_GATE_DEVICE;
    }
    else if (deviceName.contains("counter", Qt::CaseInsensitive)) // Example: Case insensitive check for "counter"
    {
        return COUNTER_DEVICE;
    }

    return DEFAULT_DEVICE;
}

sc_module* SystemCBuilder::createModule(device devType, const s_element &element)
{
    const int DEFAULT_WIDTH = 1;

    int inputs_number = static_cast<unsigned int>(element.inputs_number);

    /** /!\ TOFIX /!\
     *
     * The modules are currently not instanciables because of the template classes they use
     * We should switch to a constructor instanciation instead of a template one
     * The problem is also sc_signals that are templates and can´t have dynamic width values (W)
     *
     */

    switch (devType)
    {
    case MULTIPLEXER_DEVICE:
        return new SyscLogicGateRT(element.name.toStdString().c_str(), inputs_number, DEFAULT_WIDTH);
        // createMuxModule<2, DEBUGDEFAULTWITDTH, DEBUGDEFAULTWITDTH>(elem.name);
        break;

    case AND_GATE_DEVICE:
        return new SyscLogicGateRT(element.name.toStdString().c_str(), inputs_number, DEFAULT_WIDTH);
        // SyscAnd<2, DEBUGDEFAULTWITDTH, DEBUGDEFAULTWITDTH>(elem.name);
        break;

    case OR_GATE_DEVICE:
        return new SyscLogicGateRT(element.name.toStdString().c_str(), inputs_number, DEFAULT_WIDTH);
        // ...
        break;

    case NAND_GATE_DEVICE:
        return new SyscLogicGateRT(element.name.toStdString().c_str(), inputs_number, DEFAULT_WIDTH);
        break;

    case NOR_GATE_DEVICE:
        return new SyscLogicGateRT(element.name.toStdString().c_str(), inputs_number, DEFAULT_WIDTH);
        break;

    case XOR_GATE_DEVICE:
        return new SyscLogicGateRT(element.name.toStdString().c_str(), inputs_number, DEFAULT_WIDTH);
        break;

    case XNOR_GATE_DEVICE:
        return new SyscLogicGateRT(element.name.toStdString().c_str(), inputs_number, DEFAULT_WIDTH);
        break;
        // Handle other device types...

    default:
        break;
    }
}


