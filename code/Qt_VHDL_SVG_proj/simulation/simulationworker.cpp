#include "simulationworker.h"
#include <QThread>

SimulationWorker::SimulationWorker(QObject *parent)
    : QObject(parent)
{
    m_running = false;
    // Initialization if needed
}

void SimulationWorker::doWork()
{
    m_running = true;
    for (int i = 0; i < 5; ++i)
    {
        QThread::sleep(1); // Placeholder for the actual simulation work
        emit resultReady(QString("Step %1 completed").arg(i + 1));
    }
    emit resultReady("Simulation completed");
    QThread::sleep(2);
}

void SimulationWorker::process()
{
    if (m_running) return;

    m_running = true;
    emit workStarted();
    doWork();
    m_running = false;
    emit workFinished();
}

void SimulationWorker::stop()
{
    m_running = false;
}
