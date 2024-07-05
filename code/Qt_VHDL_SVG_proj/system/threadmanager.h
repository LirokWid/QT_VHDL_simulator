#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "simulation/simulationworker.h"

class ThreadManager : public QObject {
    Q_OBJECT

public:
    ThreadManager(QObject *parent = nullptr);
    ~ThreadManager();
    void startWork();
    void stopWork();
    void resetWorker();

signals:
    void resultReady(const QString &result);
    void simulationEnded();

private:
    QThread *thread;
    SimulationWorker *worker;
};

#endif // THREADMANAGER_H
