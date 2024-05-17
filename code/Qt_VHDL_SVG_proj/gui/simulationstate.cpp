#include "simulationstate.h"

SimulationState::SimulationState(QObject *parent) :
    QObject(parent),
    currentState(IDLE)
{
}

SimulationState::State SimulationState::getState() const
{
    return currentState;
}

void SimulationState::setState(State state)
{
    if (state != currentState)
    {
        currentState = state;
        emit stateChanged(state);
    }
}
