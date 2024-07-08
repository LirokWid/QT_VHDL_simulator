#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "simulation/simulationworker.h"

class ThreadManager : public QObject {
    Q_OBJECT

public:
    explicit ThreadManager(QObject *parent = nullptr);
    ~ThreadManager();

signals:
    void resultReady(const QString &result);
    void workStarted();
    void workFinished();
    void threadBusy();

public slots:
    void startWork();
    void handleWorkFinished();

private:
    QThread *thread;
    SimulationWorker *worker;
    bool isBusy;
};

#endif // THREADMANAGER_H
