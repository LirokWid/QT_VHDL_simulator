#include <QCoreApplication>
#include <QVector>
#include <QDebug>
//#include "sc_gates.h"
#include "sc_t_gates.h"


typedef enum svgCompType_e {
    SVG_INPUT, SVG_OUTPUT,
    SVG_AND_GATE, SVG_NAND_GATE,
    SVG_OR_GATE, SVG_NOR_GATE,
    SVG_XOR_GATE, SVG_XNOR_GATE,
    SVG_INV_GATE,
    SVG_TRI_GATE,
    SVG_MUX
} svgCompType_t;

typedef struct svgComp_s {
  uint32_t      id;     // Numerical ID
  svgCompType_t type;   // Component type
  QString       name;   // Component name
  uint32_t      N;      // Number of data inputs
  uint32_t      Win;    // Width of data inputs
  uint32_t      Wout;   // Width of data output
  uint32_t      Wsel;   // Width of sel input (MUX only)
} svgComp_t;

// Port numbers 0, 1, 2, ... correspond to data input ports;
// These macros define port numbers for outputs and other inputs
#define SVG_CONN_OUTPUT     ((uint32_t)0xFFFFFFFF)
#define SVG_CONN_MUX_SEL    ((uint32_t)0xFFFFFFFE)
#define SVG_CONN_TRI_CTL    ((uint32_t)0xFFFFFFFD)

typedef struct svgConn_s {
  uint32_t      from_id;    // Numerical ID (source)
  uint32_t      from_d;     // Numerical data input of source; -1 is output y, -2 sel for MUX / ctl for TRI
  uint32_t      to_id;      // Numerical ID (sink)
  uint32_t      to_d;       // Numerical data input of sink; -1 is output y, -2 sel for MUX / ctl for TRI
  uint32_t      W;          // Connection width in bits
  svgCompType_t from_type;
  svgCompType_t to_type;
} svgConn_t;

static uint32_t comp_id = 0;

void simAddInput(QVector<svgComp_t> &comps, QString name, uint32_t width);
void simAddOutput(QVector<svgComp_t> &comps, QString name, uint32_t width);
void simAddGate(QVector<svgComp_t> &comps, QString name, uint32_t ninputs, svgCompType_t type);
void simAddTri(QVector<svgComp_t> &comps, QString name, uint32_t width);
void simAddMux(QVector<svgComp_t> &comps, QString name, uint32_t ninputs, uint32_t width, uint32_t sel_width);
int addConnect(QVector<svgComp_t> &comps, QVector<svgConn_t> &conns, uint32_t from_id, uint32_t from_d, uint32_t to_id, uint32_t to_d);


void simAddInput(QVector<svgComp_t> &comps, QString name, uint32_t width)
{
    comps.push_back(svgComp_t{comp_id++, SVG_INPUT, name, 1, width, width, 0});
}

void simAddOutput(QList<svgComp_t> &comps, QString name, uint32_t width)
{
    comps.push_back(svgComp_t{comp_id++, SVG_OUTPUT, name, 1, width, width, 0});
}

void simAddGate(QVector<svgComp_t> &comps, QString name, uint32_t ninputs, svgCompType_t type)
{
    comps.push_back(svgComp_t{comp_id++, type, name, ninputs, 1, 1, 0});
}

void simAddTri(QVector<svgComp_t> &comps, QString name, uint32_t width)
{
    comps.push_back(svgComp_t{comp_id++, SVG_TRI_GATE, name, 1, width, width, 0});
}

void simAddMux(QVector<svgComp_t> &comps, QString name, uint32_t ninputs, uint32_t width, uint32_t sel_width)
{
    comps.push_back(svgComp_t{comp_id++, SVG_MUX, name, ninputs, width, width, sel_width});
}

int addConnect(QVector<svgComp_t> &comps, QVector<svgConn_t> &conns, uint32_t from_id, uint32_t from_d, uint32_t to_id, uint32_t to_d)
{
    uint32_t Wfrom, Wto;

    if ((from_id >= comp_id) || (to_id >= comp_id)) {
        return 1;
    }
    // Obtain source width
    Wfrom = comps[from_id].Win;
    if (from_d == SVG_CONN_OUTPUT) {
        Wfrom = comps[from_id].Wout;
    }
    if ((from_d == SVG_CONN_MUX_SEL) && (comps[from_id].type == SVG_MUX))  {
        Wfrom = comps[from_id].Wsel;
    }
    if ((from_d == SVG_CONN_TRI_CTL) && (comps[from_id].type == SVG_TRI_GATE))  {
        Wfrom = 1;
    }
    // Obtain destination width
    Wto = comps[to_id].Win;
    if (to_d == SVG_CONN_OUTPUT) {
        Wto = comps[to_id].Wout;
    }
    if ((to_d == SVG_CONN_MUX_SEL) && (comps[to_id].type == SVG_MUX))  {
        Wto = comps[to_id].Wsel;
    }
    if ((to_d == SVG_CONN_TRI_CTL) && (comps[to_id].type == SVG_TRI_GATE))  {
        Wto = 1;
    }
    if (Wfrom != Wto) {
        return 1;
    }
    conns.push_back(svgConn_t{from_id, from_d, to_id, to_d, Wfrom, comps[from_id].type, comps[to_id].type});
    return 0;
}


int simDeploy(QVector<::sc_core::sc_module *> &modules, QVector<svgComp_t> &comps);
int simConnect(QVector<::sc_core::sc_module *> &mods, QVector<::sc_core::sc_interface *> &sigs, QVector<svgConn_t> &conns);

#define DEPLOY_WOUT(modType,N,Win,Wout,name) {\
    pmod = nullptr;\
    switch(Wout) {\
    case 1: pmod = new modType(N,Win,1,name); break;\
    case 2: pmod = new modType(N,Win,2,name); break;\
    case 3: pmod = new modType(N,Win,3,name); break;\
    case 4: pmod = new modType(N,Win,4,name); break;\
    default: qDebug() << "Unsupported output signal width " << Wout; return 1;\
    }\
    }

#define DEPLOY_WIN(modType,N,Win,Wout,name) {\
    pmod = nullptr;\
    switch(Win) {\
    case 1: DEPLOY_WOUT(modType,N,1,Wout,name); break;\
    case 2: DEPLOY_WOUT(modType,N,2,Wout,name); break;\
    case 3: DEPLOY_WOUT(modType,N,3,Wout,name); break;\
    case 4: DEPLOY_WOUT(modType,N,4,Wout,name); break;\
    default: qDebug() << "Unsupported input signal width " << Win; return 1;\
    }\
    }

#define DEPLOY_SEL(modType,N,Win,Wout,Wsel,name) {\
    pmod = nullptr;\
    switch(Wsel) {\
    case 1: pmod = new modType(N,Win,Wout,Wsel,name); break;\
    case 2: pmod = new modType(N,Win,Wout,Wsel,name); break;\
    case 3: pmod = new modType(N,Win,Wout,Wsel,name); break;\
    case 4: pmod = new modType(N,Win,Wout,Wsel,name); break;\
    default: qDebug() << "Unsupported selection signal width " << Wout; return 1;\
    }\
    }

#define DEPLOY_WOUTSEL(modType,N,Win,Wout,Wsel,name) {\
    pmod = nullptr;\
    switch(Wout) {\
    case 1: DEPLOY_SEL(modType,N,Win,1,Wsel,name); break;\
    case 2: DEPLOY_SEL(modType,N,Win,2,Wsel,name); break;\
    case 3: DEPLOY_SEL(modType,N,Win,3,Wsel,name); break;\
    case 4: DEPLOY_SEL(modType,N,Win,4,Wsel,name); break;\
    default: qDebug() << "Unsupported output signal width " << Wout; return 1;\
    }\
    }

#define DEPLOY_WINSEL(modType,N,Win,Wout,Wsel,name) {\
    pmod = nullptr;\
    switch(Win) {\
    case 1: DEPLOY_WOUTSEL(modType,N,1,Wout,Wsel,name); break;\
    case 2: DEPLOY_WOUTSEL(modType,N,2,Wout,Wsel,name); break;\
    case 3: DEPLOY_WOUTSEL(modType,N,3,Wout,Wsel,name); break;\
    case 4: DEPLOY_WOUTSEL(modType,N,4,Wout,Wsel,name); break;\
    default: qDebug() << "Unsupported input signal width " << Win; return 1;\
    }\
    }


#define PORT_MAP_TYPE(m, mtype, p, w, psig) {       \
    switch (mtype) {                                \
    case SVG_INPUT:                                 \
        break;                                      \
    case SVG_OUTPUT:                                \
        break;                                      \
    case SVG_AND_GATE:                              \
        simAndTPortMap(m, p, w, psig);              \
        break;                                      \
    default:                                        \
        qDebug() << "Unknown component ID " << mtype;   \
        error++;                                      \
    }                                                   \
    }

#define PORT_MAP(m,p,w,s) {                                             \
    switch (p) {                                                        \
    case SVG_CONN_OUTPUT:                                               \
        m->y(*(dynamic_cast<::sc_core::sc_signal<sc_lv<w>> *>(s)));     \
        break;                                                          \
    case SVG_CONN_MUX_SEL:                                              \
        qDebug() << "Unsupported selection port";                       \
        break;                                                          \
    case SVG_CONN_TRI_CTL:                                              \
        qDebug() << "Unsupported tristate control port";                \
        break;                                                          \
    default:                                                            \
        (*(m->d[p]))(*(dynamic_cast<::sc_core::sc_signal<sc_lv<w>> *>(s)));  \
    }   \
    }

int simAndTPortMap(::sc_core::sc_module *m, uint32_t p, uint32_t w, ::sc_core::sc_interface *psig)
{
    switch (w) {
    case 1: PORT_MAP(dynamic_cast<SyscTAnd<1> *>(m),p,1,psig); break;
    case 2: PORT_MAP(dynamic_cast<SyscTAnd<2> *>(m),p,2,psig); break;
    case 3: PORT_MAP(dynamic_cast<SyscTAnd<3> *>(m),p,3,psig); break;
    case 4: PORT_MAP(dynamic_cast<SyscTAnd<4> *>(m),p,4,psig); break;
    default:
        qDebug() << "Unsupported signal width " << w;
        return 1;
    }
    return 0;
}


::sc_core::sc_module *simDeployAnd(uint32_t N, uint32_t W, const char *name)
{
    ::sc_core::sc_module *pmod = nullptr;
    switch (W) {
    case 1: pmod = new SyscTAnd<1>(name, N); break;
    case 2: pmod = new SyscTAnd<2>(name, N); break;
    case 3: pmod = new SyscTAnd<3>(name, N); break;
    case 4: pmod = new SyscTAnd<4>(name, N); break;
    default: qDebug() << "Unsupported width " << W;
    }
    return pmod;
}

int simDeploy(QVector<::sc_core::sc_module *> &modules, QVector<svgComp_t> &comps) {
    svgComp_t c;
    foreach(c, comps) {
        ::sc_core::sc_module *pmod = nullptr;
        switch (c.type) {
        case SVG_INPUT:
            break;
        case SVG_OUTPUT:
            break;
        case SVG_AND_GATE:
            pmod = simDeployAnd(c.N, c.Win, c.name.toStdString().c_str());
            break;
            /*
        case SVG_NAND_GATE:
            DEPLOY_WIN(SyscNand, c.N, c.Win, c.Wout, c.name.toStdString().c_str());
            break;
        case SVG_OR_GATE:
            DEPLOY_WIN(SyscOr, c.N, c.Win, c.Wout, c.name.toStdString().c_str());
            break;
        case SVG_NOR_GATE:
            DEPLOY_WIN(SyscNor, c.N, c.Win, c.Wout, c.name.toStdString().c_str());
            break;
        case SVG_XOR_GATE:
            DEPLOY_WIN(SyscXor, c.N, c.Win, c.Wout, c.name.toStdString().c_str());
            break;
        case SVG_XNOR_GATE:
            DEPLOY_WIN(SyscXNor, c.N, c.Win, c.Wout, c.name.toStdString().c_str());
            break;
        case SVG_INV_GATE:
            DEPLOY_WIN(SyscInv, c.N, c.Win, c.Wout, c.name.toStdString().c_str());
            break;
        case SVG_TRI_GATE:
            DEPLOY_WIN(SyscTri, c.N, c.Win, c.Wout, c.name.toStdString().c_str());
            break;
        case SVG_MUX:
            DEPLOY_WINSEL(SyscMux, c.N, c.Win, c.Wout, c.Wsel, c.name.toStdString().c_str());
            break;
            */
        default:
            qDebug() << "Unknown component ID " << c.type << " for '" << c.name << "'";
            return 1;
        }
        if (pmod != nullptr) {
            modules.push_back(pmod);
        }
    }
    return 0;
}

int simConnect(QVector<::sc_core::sc_module *> &mods, QVector<::sc_core::sc_interface *> &sigs, QVector<svgConn_t> &conns)
{
    int error;
    svgConn_t c;
    foreach(c, conns) {
        ::sc_core::sc_interface *psig = nullptr;
        switch (c.W) {
        case 1: psig = new ::sc_core::sc_signal<sc_lv<1>>; break;
        case 2: psig = new ::sc_core::sc_signal<sc_lv<2>>; break;
        case 3: psig = new ::sc_core::sc_signal<sc_lv<3>>; break;
        case 4: psig = new ::sc_core::sc_signal<sc_lv<4>>; break;
        default:
            qDebug() << "Unsupported signal width " << c.W;
            return 1;
        }

        if (psig != nullptr) {
            error = 0;
            PORT_MAP_TYPE(mods[c.from_id], c.from_type, c.from_d, c.W, psig);
            PORT_MAP_TYPE(mods[c.to_id], c.to_type, c.to_d, c.W, psig);
            sigs.push_back(psig);
            if (error) {
                return 1;
            }
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QVector<svgComp_t> components;  // Originally filled with SVG parsing
    QVector<svgConn_t> connections; // Originally filled with SVG parsing
    int conn_errors = 0;
    QVector<::sc_core::sc_module *> sc_modules;    // Run-time SystemC circuit (modules)
    QVector<::sc_core::sc_interface *> sc_signals; // Run-time SystemC circuit (signals)

    // PHASE 1: Data structure "from SVG"
    simAddInput(components, "i0::mux_i0", 4);       // 0: 4-bit data input 0 for u1 MUX
    simAddInput(components, "i1::mux_i1", 4);       // 1: 4-bit data input 1 for u1 MUX
    simAddInput(components, "i2::mux_or0", 1);      // 2: 1-bit data input 0 for u0 OR gate
    simAddInput(components, "i3::mux_or1", 1);      // 3: 1-bit data input 1 for u0 OR gate
    simAddOutput(components, "o0::mux_y", 4);       // 4: 4-bit data output for u1 MUX
    simAddGate(components, "u0", 2, SVG_OR_GATE);   // 5: 2-input 1-bit OR gate u0
    simAddMux(components, "u1", 2, 4, 1);           // 6: 2-input 4-bit data, 1-bit sel MUX u1

    // 0 output connected to d0 of 6
    // 1 output connected to d1 of 6
    // 2 output connected to d0 of 5
    // 3 output connected to d1 of 5
    // 5 output connected to sel input of 6
    // 6 output connected to 4 input
    conn_errors += addConnect(components, connections, 0, SVG_CONN_OUTPUT, 6, 0);
    conn_errors += addConnect(components, connections, 0, SVG_CONN_OUTPUT, 6, 0);
    conn_errors += addConnect(components, connections, 1, SVG_CONN_OUTPUT, 6, 1);
    conn_errors += addConnect(components, connections, 2, SVG_CONN_OUTPUT, 5, 0);
    conn_errors += addConnect(components, connections, 3, SVG_CONN_OUTPUT, 5, 1);
    conn_errors += addConnect(components, connections, 5, SVG_CONN_OUTPUT, 6, SVG_CONN_MUX_SEL);
    conn_errors += addConnect(components, connections, 6, SVG_CONN_OUTPUT, 4, 0);
    if (conn_errors > 0) {
        qDebug() << "Connection adding error";
    }



    // PHASE 2: Deploy SystemC components and wires

    if (simDeploy(sc_modules, components) > 0) {
        qDebug() << "Components error";
    }
    if (simConnect(sc_modules, sc_signals, connections) > 0) {
        qDebug() << "Connections error";
    }

    // PHASE 3: Simulate
    sc_start(30, SC_NS);
    for (int i = 0; i < 4; i++) {
        sc_start(10, SC_NS);
    }
    return a.exec();
}
