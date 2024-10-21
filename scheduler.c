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

/*
static void setBursts(Process *p)
{
    FILE *random_num_file = fopen("random-numbers", "r");
    p->CPUBurst = randomOS(p->B, p->processID, random_num_file);
    p->CPUBurst = p->CPUBurst * p->M;
    fclose(random_num_file);
}
*/

int main(int argc, char *argv[])
{
    // Check for correct number of arguments
    if (argc != 2)
    {
        printf("Error: Usage: ./scheduler <input-file>\n");
        return 1;
    }

    SchedulerStats stats = initStats();

    // Open the file
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    // populate the process list
    Process *p_list = readAllProcessesFromFile(input_file, &stats);

    printStart(p_list, stats);
    fclose(input_file);

    return 0;
}