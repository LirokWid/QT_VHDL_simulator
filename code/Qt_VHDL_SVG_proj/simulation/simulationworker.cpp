#include "simulationworker.h"
#include "svghandler.h"
#include <QThread>
//#include "SystemcLinker.h" // to change to another thing


SimulationWorker::SimulationWorker(QObject *parent)
    : QObject(parent)
{
    m_running = false;
    // Initialization if needed
}

void SimulationWorker::doWork(WorkType workType)
{
    for (int i = 0; i < 100; ++i)
    {
        QThread::msleep(25); // Placeholder for the actual simulation work
        emit resultReady(QString("Step %1 completed").arg(i + 1));
    }

    emit resultReady("Simulation completed");
    QThread::sleep(2);
}

void SimulationWorker::simulationStart()
{
    //Faut récupérer l'instance de svg handler
    //1. Analyse parsed file and create system-c simulation
    m_components = SvgHandler::getComponentsList();
    if (!m_components.isEmpty())
    {
        for (const auto& elem : m_components.elements.elements_list)
        {
            qDebug() << elem.name;
        }
        for (const auto& elem : m_components.simulation_IOs.i_os)
        {
            qDebug() << elem.name;
        }
        for (const auto& elem : m_components.simulation_wires.wires)
        {
            qDebug() << elem.name;
        }
    }
    else
    {
        qDebug() << "Simulation aborted, empty component list";
        simulationStop();
    }
    simulationStop();
}

void SimulationWorker::simulationStop()
{
    m_running = false;
    emit workFinished();
}
void SimulationWorker::process(WorkType workType)
{
    if (m_running) return;

    m_running = true;
    emit workStarted();
    doWork(workType);
    m_running = false;
    emit workFinished();
}

void SimulationWorker::stop()
{
    m_running = false;
}

void SimulationWorker::buildSimulation()
{

}
