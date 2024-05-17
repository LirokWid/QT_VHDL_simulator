/**
 * @file sc_run.h
 * @brief SystemC simulation control interface
 *
 * This file contains the SystemC simulation control interface.
 *
 * The interface provides a way to control the simulation execution from a separate thread.
 * The simulation control interface is used to start and stop the simulation, and to get the simulation results.
 *
 * The interface is implemented using a SystemC thread that waits for commands from a separate thread.
 * The SystemC thread is controlled by the simulation control interface functions.
*/

#ifndef SC_RUN_H
#define SC_RUN_H

#include <pthread.h>
#include <vector>
#include <string>

#define SC_ST_COMMAND_RUN           0       // Used by UI to run a simulation microstep
#define SC_ST_COMMAND_EXIT          1       // Used by UI to finish the simulation (sc_stop)
#define SC_ST_RESPONSE_STOP         99      // Used by SystemC to signal it is waiting for the UI
#define SC_ST_RESPONSE_FINISHED     100     // Used by SystemC to signal it has finished
#define SC_ST_RESPONSE_ABORTED      101     // Used by SystemC to signal an internal error

#define SC_ST_MAXCOMMAND            50      // No commands (SC_ST_COMMAND_XXX above this value)
#define SC_ST_MINRESPONSE           60      // No responses (SC_ST_RESPONSE_XXX below this value)


typedef struct sc_thread_run_s
{
    int                         state;          // One of the #define's SC_ST_XXX values
    std::vector<std::string>    hierarchy;      // Hierarchy of the SystemC module
    std::vector<std::string>    tracelist;      // List of signals to trace
    std::vector<std::string>    report;         // Report of the simulation
    pthread_mutex_t             run_mutex;      // Mutex to control the execution of the SystemC thread
    pthread_mutex_t             condvar_mutex;  // Mutex for the condition variable
    pthread_cond_t              condvar;        // Condition variable that controls SystemC thread execution
} sc_thread_run_t;

/*
 * To be called from UI simulation control thread
 */
int sc_StartTransactionFromUI();
int sc_EndTransactionFromUI();
int sc_NewStateFromUI(int newState = SC_ST_COMMAND_RUN);
int sc_getRunResultFromUI(void);
void sc_setRunFromUI(sc_thread_run_t *theRun);
std::vector<std::string> sc_getHierarchyFromUI();
void sc_setTraceListFromUI(std::vector<std::string> &tl);
std::vector<std::string> sc_getReportFromUI();

/*
 * To be called from SystemC simulation thread
 */
void sc_NotifyUIFromSC(int newState = SC_ST_RESPONSE_STOP);
int  sc_waitStateChange(void);
void sc_setRunResult(int theResult);
void sc_setReportFromSC(std::vector<std::string> &rep);
void sc_setHierarchyFromSC(std::vector<std::string> &hier);

#endif // SC_RUN_H
