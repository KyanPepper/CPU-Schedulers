#include "scheduler.h"

const char *RANDOM_NUMBER_FILE_NAME = "random-numbers";

extern uint32_t getRandNumFromFile(uint32_t line, FILE *random_num_file_ptr);

extern uint32_t randomOS(uint32_t upper_bound, uint32_t process_indx, FILE *random_num_file_ptr);

extern void printStart(Process process_list[], SchedulerStats stats);

extern void printFinal(Process finished_process_list[], SchedulerStats stats);

extern void printProcessSpecifics(Process process_list[], SchedulerStats stats);