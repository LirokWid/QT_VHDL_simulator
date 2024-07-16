#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>
#include "ComponentsStruct.h"
#include "SystemcLinker.h"
#include "simulationstate.h"


class SimulationWorker : public QObject {
    Q_OBJECT

public:
    explicit SimulationWorker(QObject *parent = nullptr);

    enum class WorkType {
        LINK,
        SIMULATE
    };
    void doWork(WorkType type);

    void simulationStart();
    void simulationStop();
public slots:
    void process(WorkType workType);
    void stop();

signals:
    void resultReady(const QString &result);
    void workFinished();
    void workStarted();

private:
    bool m_running = false;
    void buildSimulation();

    SystemcLinker *linker;

    s_components_list m_components;


};

#endif // SIMULATIONWORKER_H
