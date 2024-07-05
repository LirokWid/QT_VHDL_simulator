#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>

class SimulationWorker : public QObject {
    Q_OBJECT

public:
    SimulationWorker(QObject *parent = nullptr);

    bool getIsActive() const;

public slots:
    void doWork();
    void reset();

signals:
    void resultReady(const QString &result);
    void simulationEnded();

private:
    bool isActive = false;
};

#endif // SIMULATIONWORKER_H
