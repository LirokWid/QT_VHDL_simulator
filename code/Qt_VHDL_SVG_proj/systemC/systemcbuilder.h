#ifndef SYSTEMCBUILDER_H
#define SYSTEMCBUILDER_H

#include "ComponentsStruct.h"
#include "debugwindow.h"
#include "systemC/systemc_modules/sc_gates.h"
#include "systemC/systemc_modules/sc_mux.h"

typedef enum e_device
{
    MULTIPLEXER_DEVICE,
    AND_GATE_DEVICE,
    OR_GATE_DEVICE,
    NAND_GATE_DEVICE,
    NOR_GATE_DEVICE,
    XOR_GATE_DEVICE,
    XNOR_GATE_DEVICE,
    COUNTER_DEVICE,
    DEFAULT_DEVICE
}device;

class SystemCBuilder
{
public:
    SystemCBuilder(s_components_list *components);

private:
    template <unsigned int N, unsigned int W, unsigned int SELW>
    SyscMux<N, W, SELW>* createMuxModule(const QString& name);

    template <template <unsigned int, unsigned int> class GateType, unsigned int N, unsigned int W>
    GateType<N, W>* createGateModule(const QString& name);

    DebugWindow *debug = DebugWindow::getInstance();
    s_components_list *components;
    QVector<sc_module*> modules;

    void buildSystemCSim(const s_components_list& components);

    device deviceFinder(QString name);


    void createModule(device, const s_element &);
};

#endif // SYSTEMCBUILDER_H
