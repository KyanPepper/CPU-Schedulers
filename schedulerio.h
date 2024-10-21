
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Process
{
    uint32_t A;         // A: Arrival time of the process
    uint32_t B;         // B: Upper Bound of CPU burst times of the given random integer list
    uint32_t C;         // C: Total CPU time required
    uint32_t M;         // M: Multiplier of CPU burst time
    uint32_t processID; // The process ID given upon input read

    uint8_t status; // 0 is not started, 1 is ready, 2 is running, 3 is blocked, 4 is terminated

    int32_t finishingTime;         // The cycle when the the process finishes (initially -1)
    uint32_t currentCPUTimeRun;    // The amount of time the process has already run (time in running state)
    uint32_t currentIOBlockedTime; // The amount of time the process has been IO blocked (time in blocked state)
    uint32_t currentWaitingTime;   // The amount of time spent waiting to be run (time in ready state)

    uint32_t IOBurst;  // The amount of time until the process finishes being blocked
    uint32_t CPUBurst; // The CPU availability of the process (has to be > 1 to move to running)

    int32_t quantum; // Used for schedulers that utilize pre-emotion

    int isFirstTimeRunning; // Used to check when to calculate the CPU burst when it hits running mode
} Process;

typedef struct SchedulerStats
{
    uint32_t CURRENT_CYCLE;                        // The current cycle that each process is on
    uint32_t TOTAL_CREATED_PROCESSES;              // The total number of processes constructed
    uint32_t TOTAL_STARTED_PROCESSES;              // The total number of processes that have started being simulated
    uint32_t TOTAL_FINISHED_PROCESSES;             // The total number of processes that have finished running
    uint32_t TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED; // The total cycles in the blocked state
} SchedulerStats;

extern SchedulerStats initStats();

/**
 * Prints to standard output the start of the output
 * process_list is the original processes inputted, in array form
 */
extern void printStart(Process process_list[], SchedulerStats stats);

/**
 * Prints to standard output the final output
 * finished_process_list is the terminated processes (in array form) in the order they each finished in.
 */
extern void printFinal(Process finished_process_list[], SchedulerStats stats);

/**
 * Prints out specifics for each process.
 * @param process_list The original processes inputted, in array form
 */
extern void printProcessSpecifics(Process process_list[], SchedulerStats stats);

/**
 * Prints out the summary data
 * process_list The original processes inputted, in array form
 */
extern void printSummaryData(Process process_list[], SchedulerStats stats);

extern void printCycle(Process process_list[], SchedulerStats stats);

// Creates a process array in memory from the input file
extern Process* readAllProcessesFromFile(FILE *input_file, SchedulerStats *stats);