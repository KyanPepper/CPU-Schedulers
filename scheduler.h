#include "rand.h"
#include "schedulerio.h"


// First Come First Serve
extern void fcfs(Process *p_list, SchedulerStats *stats);

// Round Robin
extern void rr(Process *p_list, SchedulerStats *stats);

// Shortest Job First
extern void sjf(Process *p_list, SchedulerStats *stats);
