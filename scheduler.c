#include "scheduler.h"

SchedulerStats initStats()
{
    SchedulerStats stats;
    stats.CURRENT_CYCLE = 0;
    stats.TOTAL_CREATED_PROCESSES = 0;
    stats.TOTAL_STARTED_PROCESSES = 0;
    stats.TOTAL_FINISHED_PROCESSES = 0;
    stats.TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;
    return stats;
}

static void setBursts(Process *p)
{
    FILE *random_num_file = fopen("random-numbers", "r");
    if (random_num_file == NULL)
    {
        printf("Error: Cannot open file random-numbers\n");
        exit(1);
    }
    // Set the CPU burst time
    p->CPUBurst = randomOS(p->B, p->processID, random_num_file);
    // Set the IO burst time
    p->IOBurst = p->M * p->CPUBurst;

    fclose(random_num_file);
}

void fcfs(Process *p_list, SchedulerStats *stats)
{

    Process *rp = NULL; // Running process
    Process *p = NULL;  // Process
    printf("**** \n\n\n\nFirst Come First Serve\n");
    while (stats->TOTAL_FINISHED_PROCESSES < stats->TOTAL_CREATED_PROCESSES)
    {

        for (uint32_t i = 0; i < stats->TOTAL_CREATED_PROCESSES; i++)
        {
            uint32_t index = i;
            if (rp != NULL)
            {
                index = (rp - p_list) + i;
            }

            if (index >= stats->TOTAL_CREATED_PROCESSES) // Wrap the index if it goes beyond the number of processes
            {
                index -= stats->TOTAL_CREATED_PROCESSES; // Adjust index to wrap around to the beginning
            }

            p = &p_list[index];

            // Terminated
            if (p->status == TERMINATED)
            {
                continue;
            }

            // Blocked
            if (p->status == BLOCKED)
            {
                p->currentIOBlockedTime++;
                p->IOBurst--;

                // if the process is done being blocked
                if (p->IOBurst <= 0)
                {
                    p->status = READY;
                }

                stats->TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED++;
            }
            // if the process is not started and the arrival time is the current cycle
            else if (p->status == NOT_STARTED)
            {
                p->status = READY;
                stats->TOTAL_STARTED_PROCESSES++;
                p->isFirstTimeRunning = 1;
            }
            if (p->status == READY)
            {
                // Ready -> Running
                if (rp == NULL)
                {
                    setBursts(p);
                    p->status = RUNNING;
                    rp = p;
                }
                else
                {
                    p->currentWaitingTime++;
                }
            }
            // Running
            else if (p->status == RUNNING)
            {
                p->currentCPUTimeRun++;
                p->CPUBurst--;
                stats->TOTAL_STARTED_PROCESSES += p->isFirstTimeRunning;
                p->isFirstTimeRunning = 0;

                if (p->currentCPUTimeRun >= p->C)
                {
                    p->status = TERMINATED;
                    p->finishingTime = stats->CURRENT_CYCLE;
                    rp = NULL;

                    stats->TOTAL_FINISHED_PROCESSES++;
                }

                // Running -> Block
                else if (p->CPUBurst <= 0)
                {
                    rp = NULL;
                    p->status = BLOCKED;
                }
            }
        }

        stats->CURRENT_CYCLE++;
    }
    printStart(p_list, *stats);
    printFinal(p_list, *stats);
    printProcessSpecifics(p_list, *stats);
    printSummaryData(p_list, *stats);
}

void sjf(Process *p_list, SchedulerStats *stats)
{

    printf("**** \n\n\n\n Shortest Job First\n");
    Process *p = NULL;
    while (stats->TOTAL_FINISHED_PROCESSES < stats->TOTAL_CREATED_PROCESSES)
    {
        Process *s = NULL;

        for (uint32_t i = 0; i < stats->TOTAL_CREATED_PROCESSES; i++)
        {
            p = &p_list[i];

            // Terminated
            if (p->status == TERMINATED)
            {
                continue;
            }

            // Blocked
            if (p->status == BLOCKED)
            {
                p->currentIOBlockedTime++;
                p->IOBurst--;

                // Blocked -> Ready
                if (p->IOBurst <= 0)
                {
                    p->status = READY;
                }

                stats->TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED++;
            }

            // Unstarted -> Ready
            else if (p->status == NOT_STARTED && p->A == stats->CURRENT_CYCLE)
            {
                p->status = READY;
                stats->TOTAL_STARTED_PROCESSES++;
                p->isFirstTimeRunning = 1;
            }

            // Ready
            if (p->status == READY)
            {
                if (s == NULL)
                {
                    s = p; // If sj is NULL, assign p to sj
                }
                else
                {
                    // Compare the remaining CPU time of p and sj
                    if ((p->C - p->currentCPUTimeRun) < (s->C - s->currentCPUTimeRun))
                    {
                        s = p; // If p has less remaining CPU time than sj, assign p to sj
                    }
                }

                p->currentWaitingTime++;
            }

            // Running
            else if (p->status == RUNNING)

            {
                p->currentCPUTimeRun++;
                p->CPUBurst--;
                stats->TOTAL_STARTED_PROCESSES += p->isFirstTimeRunning;
                p->isFirstTimeRunning = 0;

                // Running -> Terminate
                if (p->currentCPUTimeRun >= p->C)
                {
                    p->status = TERMINATED;
                    p->finishingTime = stats->CURRENT_CYCLE;

                    stats->TOTAL_FINISHED_PROCESSES++;
                }

                // Running -> Block
                else if (p->CPUBurst <= 0)
                {
                    p->status = BLOCKED;
                }
            }
        }

        // Ready -> Running for Shortest Job
        if (s != NULL)
        {
            setBursts(s);
            s->status = RUNNING;
            s->currentWaitingTime--;
        }

        stats->CURRENT_CYCLE++;
    }
    printStart(p_list, *stats);
    printFinal(p_list, *stats);
    printProcessSpecifics(p_list, *stats);
    printSummaryData(p_list, *stats);
}

void rr(Process *p_list, SchedulerStats *stats)
{
    printf("**** \n\n\n\n Round Robin\n");

    Process *rp = NULL; // Running process
    Process *p = NULL;  // Process

    while (stats->TOTAL_FINISHED_PROCESSES < stats->TOTAL_CREATED_PROCESSES)
    {

        for (uint32_t i = 0; i < stats->TOTAL_CREATED_PROCESSES; i++)
        {
            uint32_t index = i;
            if (rp != NULL)
            {
                index = (rp - p_list) + i;
            }

            if (index >= stats->TOTAL_CREATED_PROCESSES) // Wrap the index if it goes beyond the number of processes
            {
                index -= stats->TOTAL_CREATED_PROCESSES; // Adjust index to wrap around to the beginning
            }

            p = &p_list[index];
            // Terminated
            if (p->status == TERMINATED)
            {
                continue;
            }

            // Blocked
            if (p->status == BLOCKED)
            {
                p->currentIOBlockedTime++;
                p->IOBurst--;

                // Blocked -> Ready
                if (p->IOBurst <= 0)
                {
                    p->status = READY;
                }

                stats->TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED++;
            }

            // Not started -> Ready
            else if (p->status == NOT_STARTED)
            {

                p->status = READY;
                stats->TOTAL_STARTED_PROCESSES++;
                p->isFirstTimeRunning = 1;
            }

            // Ready
            if (p->status == READY)
            {
                // Ready -> Running
                if (rp == NULL)
                {
                    setBursts(p);
                    p->status = RUNNING;
                    rp = p;
                }
                else
                {
                    p->currentWaitingTime++;
                }
            }

            // Running
            else if (p->status == RUNNING)
            {
                p->currentCPUTimeRun++;
                p->CPUBurst--;
                stats->TOTAL_STARTED_PROCESSES += p->isFirstTimeRunning;
                p->isFirstTimeRunning = 0;

                // Running -> Terminate
                if (p->currentCPUTimeRun >= p->C)
                {
                    p->status = TERMINATED;
                    p->finishingTime = stats->CURRENT_CYCLE;
                    rp = NULL;

                    stats->TOTAL_FINISHED_PROCESSES++;
                }

                // Running -> Block
                else if (p->CPUBurst <= 0)
                {
                    rp = NULL;
                    p->status = BLOCKED;
                }
                // Running -> Ready
                else if (p->quantum > 0 && p->currentCPUTimeRun % p->quantum == 0)
                {
                    p->status = READY;
                    rp = NULL;
                }
            }
        }

        stats->CURRENT_CYCLE++;
    }
    printStart(p_list, *stats);
    printFinal(p_list, *stats);
    printProcessSpecifics(p_list, *stats);
    printSummaryData(p_list, *stats);
}

// Comparator function for sorting by arrival time, with tiebreaker by process ID
int compareArrivalTime(const void *a, const void *b)
{

    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;

    if (p1->A != p2->A)
    {
        return p1->A - p2->A;
    }

    return p1->processID - p2->processID;
}

int main(int argc, char *argv[])
{
    // Check for correct number of arguments
    if (argc != 2)
    {
        printf("Error: Usage: ./scheduler <input-file>\n");
        return 1;
    }
    // Open the file
    FILE *stream1 = fopen(argv[1], "r");
    FILE *stream2 = fopen(argv[1], "r");
    FILE *stream3 = fopen(argv[1], "r");
    if (stream1 == NULL)
    {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    // Initialize the stats
    SchedulerStats fcfs_stats = initStats();
    SchedulerStats sjf_stats = initStats();
    SchedulerStats rr_stats = initStats();

    // Read all processes from the file
    Process *fcfs_list = readAllProcessesFromFile(stream1, &fcfs_stats);
    Process *sjf_list = readAllProcessesFromFile(stream2, &sjf_stats);
    Process *rr_list = readAllProcessesFromFile(stream3, &rr_stats);
    fclose(stream1);
    fclose(stream2);
    fclose(stream3);

    // Run the FCFS scheduler
    qsort(fcfs_list, fcfs_stats.TOTAL_CREATED_PROCESSES, sizeof(Process), compareArrivalTime);
    fcfs(fcfs_list, &fcfs_stats);
    qsort(sjf_list, sjf_stats.TOTAL_CREATED_PROCESSES, sizeof(Process), compareArrivalTime);
    sjf(sjf_list, &sjf_stats);
    qsort(rr_list, rr_stats.TOTAL_CREATED_PROCESSES, sizeof(Process), compareArrivalTime);
    rr(rr_list, &rr_stats);

    return 0;
}