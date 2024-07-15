#include "simulationmanager.h"

SimulationManager::SimulationManager(QObject *parent)
    : QObject(parent), worker(new SimulationWorker)
{

    state = SimulationState::instance();
    isBusy = false;
    simThread = new QThread();

    qDebug() << "Simulation thread created at " << &simThread;
    worker = new SimulationWorker();
    worker->moveToThread(simThread);


    connect(worker, &SimulationWorker::resultReady, this, &SimulationManager::resultReady);
    connect(worker, &SimulationWorker::workStarted, this, &SimulationManager::workStarted);
    connect(worker, &SimulationWorker::workFinished, this, &SimulationManager::handleWorkFinished);


    simThread->start();
}

SimulationManager::~SimulationManager()
{
    simThread->quit();
    simThread->wait();
    delete worker;
}

void SimulationManager::lauchSimulation()
{
    if (state->getState() == SimulationState::IDLE_SVG_LOADED)
    {
        startWork();
        state->setState(SimulationState::RUNNING);
    }
    else
    {
        qDebug()<< "Sim cannot launch with incorect state";
    }
}

void SimulationManager::stopSimulation()
{
    if (state->getState() == SimulationState::RUNNING)
    {
        //resetSvgUi();
        //stopWork();
        state->setState(SimulationState::IDLE_SVG_LOADED);
    }
    else
    {
        qDebug()<< "Sim cannot stop with incorect state";
    }
}

void SimulationManager::startWork()
{
    if (isBusy)
    {
        emit threadBusy();
    }
    else
    {
        isBusy = true;
    }
}


void SimulationManager::handleWorkFinished()
{
    isBusy = false;
    emit workFinished();
}
