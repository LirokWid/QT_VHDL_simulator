#include "threadmanager.h"
#include "qdebug.h"

ThreadManager::ThreadManager(QObject *parent)
    : QObject(parent), worker(new SimulationWorker)
{

    isBusy = false;
    thread = new QThread();
    worker = new SimulationWorker();
    worker->moveToThread(thread);


    connect(worker, &SimulationWorker::resultReady, this, &ThreadManager::resultReady);
    connect(worker, &SimulationWorker::workStarted, this, &ThreadManager::workStarted);
    connect(worker, &SimulationWorker::workFinished, this, &ThreadManager::handleWorkFinished);
    //connect(worker, &SimulationWorker::workFinished, thread, &QThread::quit);

    //connect(thread, &QThread::started, worker, &SimulationWorker::doWork);
    //connect(worker, &SimulationWorker::resultReady, this, &ThreadManager::resultReady);
    //connect(worker, &SimulationWorker::simulationEnded, this, &ThreadManager::resetWorker);
    thread->start();
}

ThreadManager::~ThreadManager()
{
    thread->quit();
    thread->wait();
    delete worker;
}

void ThreadManager::startWork()
{
    if (isBusy)
    {
        emit threadBusy();
    }
    else
    {
        isBusy = true;
        QMetaObject::invokeMethod(worker, "process");// Thread safe call
    }
}


void ThreadManager::handleWorkFinished()
{
    isBusy = false;
    emit workFinished();
}
