#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include <QObject>
#include <QThread>
#include "simulation/simulationworker.h"
#include "simulationstate.h"

class SimulationManager : public QObject
{
    Q_OBJECT

public:
    explicit SimulationManager(QObject *parent = nullptr);
    ~SimulationManager();

    void lauchSimulation();
    void stopSimulation();
signals:
    void resultReady(const QString &result);
    void workStarted();
    void workFinished();
    void threadBusy();

public slots:
    void handleWorkFinished();

private:
    SimulationState *state;
    QThread *simThread;
    SimulationWorker *worker;
    bool isBusy;

    void startWork();
    bool startThread();
    void stopThread();

};

#endif // SIMULATIONMANAGER_H
