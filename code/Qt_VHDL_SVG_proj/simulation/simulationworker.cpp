#include "simulationworker.h"
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
    switch(workType)
    {
    case WorkType::LINK:
        //Call linker class to link the components
        //linker = new SystemcLinker();

        break;
    case WorkType::SIMULATE:
        break;
    }

    m_running = true;
    for (int i = 0; i < 100; ++i)
    {
        QThread::msleep(25); // Placeholder for the actual simulation work
        emit resultReady(QString("Step %1 completed").arg(i + 1));
    }
    emit resultReady("Simulation completed");
    QThread::sleep(2);
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
