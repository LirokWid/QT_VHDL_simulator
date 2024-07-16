#include "simulationmanager.h"

SimulationManager::SimulationManager(QObject *parent)
    : QObject(parent), worker(new SimulationWorker)
{

    state = SimulationState::instance();
    isBusy = false;




    connect(worker, &SimulationWorker::resultReady, this, &SimulationManager::resultReady);
    connect(worker, &SimulationWorker::workStarted, this, &SimulationManager::workStarted);
    connect(worker, &SimulationWorker::workFinished, this, &SimulationManager::handleWorkFinished);


    //simThread->start();
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
    {// TODO cancel simulation if parsing is incorect
        startThread();
        worker->simulationStart();
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
        stopThread();
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

bool SimulationManager::startThread()
{
    if (simThread != nullptr)
    {
        simThread = new QThread();
        worker = new SimulationWorker();
        worker->moveToThread(simThread);
        qDebug() << "Simulation thread created at " << &simThread;
        return true;
    }
    else
    {
        qDebug() << "Tried creating thread but one already exist";
        return false;
    }
    return false;
}

void SimulationManager::stopThread()
{
    if (simThread != nullptr)
    {
        simThread->quit();
        simThread->wait();
        delete worker;
    }
}


void SimulationManager::handleWorkFinished()
{
    isBusy = false;
    stopThread();
}
