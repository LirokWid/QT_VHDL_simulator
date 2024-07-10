#include "simulationstate.h"

SimulationState* SimulationState::m_instance = nullptr;
QMutex SimulationState::m_mutex;

SimulationState::SimulationState(QObject *parent) :
    QObject(parent),
    currentState(IDLE)
{
}

SimulationState* SimulationState::instance()
{
    if (!m_instance) {
        QMutexLocker locker(&m_mutex);
        if (!m_instance) {
            m_instance = new SimulationState();
        }
    }
    return m_instance;
}

SimulationState::State SimulationState::getState() const
{
    QMutexLocker locker(&m_stateMutex);
    return currentState;
}

void SimulationState::setState(State state)
{
    {
        QMutexLocker locker(&m_stateMutex);
        if (state != currentState)
        {
            currentState = state;
        }
    }
    emit stateChanged(state);
}
