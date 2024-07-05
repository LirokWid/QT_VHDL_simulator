#include "threadmanager.h"
#include "qdebug.h"

ThreadManager::ThreadManager(QObject *parent)
    : QObject(parent), thread(new QThread), worker(new SimulationWorker)
{

    worker->moveToThread(thread);

    //connect(thread, &QThread::started, worker, &SimulationWorker::doWork);
    connect(worker, &SimulationWorker::resultReady, this, &ThreadManager::resultReady);
    connect(worker, &SimulationWorker::simulationEnded, worker, &SimulationWorker::reset);
    thread->start();
}

ThreadManager::~ThreadManager()
{
    stopWork();
    delete worker;
    delete thread;
}

void ThreadManager::startWork()
{
    if (!worker->getIsActive())
    {
        worker->doWork();
    }
    else
    {
        qDebug() << "thread already running!";
    }
}

void ThreadManager::stopWork()
{
    if (thread->isRunning())
    {
        thread->quit();
        thread->wait();
    }
    else
    {
        qDebug() << "sim already stopped";
    }
}

void ThreadManager::resetWorker()
{
    worker->reset();
}
