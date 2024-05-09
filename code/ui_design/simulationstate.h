#ifndef SIMULATIONSTATE_H
#define SIMULATIONSTATE_H

#include <QObject>

/**
 * @brief The SimulationState class manages the state of the simulation.
 */
class SimulationState : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief The State enum represents different states of the simulation.
     */
    enum State {
        IDLE, /**< The simulation is idle. */
        IDLE_SVG_LOADED, /**< The simulation is idle with an SVG file loaded. */
        RUNNING /**< The simulation is running. */
    };

    /**
     * @brief Constructs a SimulationState object.
     * @param parent The parent object.
     */
    explicit SimulationState(QObject *parent = nullptr);

    /**
     * @brief Gets the current state of the simulation.
     * @return The current state of the simulation.
     */
    State getState() const;

public slots:
    /**
     * @brief Sets the state of the simulation.
     * @param state The new state of the simulation.
     */
    void setState(State state);

signals:
    /**
     * @brief Signal emitted when the state of the simulation changes.
     * @param newState The new state of the simulation.
     */
    void stateChanged(State newState);

private:
    State currentState; /**< The current state of the simulation. */
};

#endif // SIMULATIONSTATE_H
