#include "schedulerio.h"
/********************* SOME PRINTING HELPERS *********************/

void printStart(Process process_list[], SchedulerStats stats)
{
    printf("The original input was: %i", stats.TOTAL_CREATED_PROCESSES);

    uint32_t i = 0;
    for (; i < stats.TOTAL_CREATED_PROCESSES; ++i)
    {
        printf(" ( %i %i %i %i)", process_list[i].A, process_list[i].B,
               process_list[i].C, process_list[i].M);
    }
    printf("\n");
}

void printFinal(Process finished_process_list[], SchedulerStats stats)
{
    printf("The (sorted) input is: %i", stats.TOTAL_CREATED_PROCESSES);

    uint32_t i = 0;
    for (; i < stats.TOTAL_FINISHED_PROCESSES; ++i)
    {
        printf(" ( %i %i %i %i)", finished_process_list[i].A, finished_process_list[i].B,
               finished_process_list[i].C, finished_process_list[i].M);
    }
    printf("\n");
} // End of the print final function

void printProcessSpecifics(Process process_list[], SchedulerStats stats)
{
    uint32_t i = 0;
    printf("\n");
    for (; i < stats.TOTAL_CREATED_PROCESSES; ++i)
    {
        printf("Process %i:\n", process_list[i].processID);
        printf("\t(A,B,C,M) = (%i,%i,%i,%i)\n", process_list[i].A, process_list[i].B,
               process_list[i].C, process_list[i].M);
        printf("\tFinishing time: %i\n", process_list[i].finishingTime);
        printf("\tTurnaround time: %i\n", process_list[i].finishingTime - process_list[i].A);
        printf("\tI/O time: %i\n", process_list[i].currentIOBlockedTime);
        printf("\tWaiting time: %i\n", process_list[i].currentWaitingTime);
        printf("\n");
    }
} // End of the print process specifics function

void printSummaryData(Process process_list[], SchedulerStats stats)
{
    uint32_t i = 0;
    double total_amount_of_time_utilizing_cpu = 0.0;
    double total_amount_of_time_io_blocked = 0.0;
    double total_amount_of_time_spent_waiting = 0.0;
    double total_turnaround_time = 0.0;
    uint32_t final_finishing_time = stats.CURRENT_CYCLE - 1;
    for (; i < stats.TOTAL_CREATED_PROCESSES; ++i)
    {
        total_amount_of_time_utilizing_cpu += process_list[i].currentCPUTimeRun;
        total_amount_of_time_io_blocked += process_list[i].currentIOBlockedTime;
        total_amount_of_time_spent_waiting += process_list[i].currentWaitingTime;
        total_turnaround_time += (process_list[i].finishingTime - process_list[i].A);
    }

    // Calculates the CPU utilization
    double cpu_util = total_amount_of_time_utilizing_cpu / final_finishing_time;

    // Calculates the IO utilization
    double io_util = (double)stats.TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED / final_finishing_time;

    // Calculates the throughput (Number of processes over the final finishing time times 100)
    double throughput = 100 * ((double)stats.TOTAL_CREATED_PROCESSES / final_finishing_time);

    // Calculates the average turnaround time
    double avg_turnaround_time = total_turnaround_time / stats.TOTAL_CREATED_PROCESSES;

    // Calculates the average waiting time
    double avg_waiting_time = total_amount_of_time_spent_waiting / stats.TOTAL_CREATED_PROCESSES;

    printf("Summary Data:\n");
    printf("\tFinishing time: %i\n", stats.CURRENT_CYCLE - 1);
    printf("\tCPU Utilization: %6f\n", cpu_util);
    printf("\tI/O Utilization: %6f\n", io_util);
    printf("\tThroughput: %6f processes per hundred cycles\n", throughput);
    printf("\tAverage turnaround time: %6f\n", avg_turnaround_time);
    printf("\tAverage waiting time: %6f\n", avg_waiting_time);
    printf("total amount of time blocked: %6f\n", total_amount_of_time_io_blocked);
} // End of the print summary data function

// Allocate memory for a new process and return a pointer to it
Process createProcess(FILE *input)
{
    Process process;
    fscanf(input, " (%u %u %u %u)", &process.A, &process.B, &process.C, &process.M);
    process.status = NOT_STARTED;
    process.finishingTime = -1;
    process.currentCPUTimeRun = 0;
    process.currentIOBlockedTime = 0;
    process.currentWaitingTime = 0;
    process.IOBurst = 0;
    process.CPUBurst = 0;
    process.quantum = 2;
    process.isFirstTimeRunning = 1;
    return process;
}

Process *readAllProcessesFromFile(FILE *input_file, SchedulerStats *stats)
{
    int numProcesses = 0;
    fscanf(input_file, "%d", &numProcesses);
    Process *process_list = (Process *)malloc((numProcesses) * sizeof(Process));

    // Populate the array with processes from the file
    for (int i = 0; i < numProcesses; i++)
    {
        process_list[i] = createProcess(input_file);
        process_list[i].processID = i; // Assign a process ID based on index
    }

    stats->TOTAL_CREATED_PROCESSES = numProcesses;

    return process_list;
}