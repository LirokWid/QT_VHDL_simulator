#include "simulationworker.h"
#include <QThread>

SimulationWorker::SimulationWorker(QObject *parent)
    : QObject(parent)
{
    // Initialization if needed
}

void SimulationWorker::doWork()
{
    isActive = true;
    for (int i = 0; i < 5; ++i)
    {
        QThread::sleep(1); // Placeholder for the actual simulation work
        emit resultReady(QString("Step %1 completed").arg(i + 1));
    }
    emit resultReady("Simulation completed");
    QThread::sleep(2);
    emit simulationEnded();
}

void SimulationWorker::reset()
{
    // Reset any internal state if necessary
    emit resultReady(QString("sim stopped"));
    isActive = false;
}

bool SimulationWorker::getIsActive() const
{
    return isActive;
}
