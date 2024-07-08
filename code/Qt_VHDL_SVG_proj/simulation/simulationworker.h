#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>

class SimulationWorker : public QObject {
    Q_OBJECT

public:
    explicit SimulationWorker(QObject *parent = nullptr);

    void doWork();

public slots:
    void process();
    void stop();

signals:
    void resultReady(const QString &result);
    void workFinished();
    void workStarted();

private:
    bool m_running = false;
};

#endif // SIMULATIONWORKER_H
