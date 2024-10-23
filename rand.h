#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
/**
 * Reads a random non-negative integer X from a file named random-numbers.
 * Returns the CPU Burst: : 1 + (random-number-from-file % upper_bound)
 */
extern uint32_t randomOS(uint32_t upper_bound, uint32_t process_indx, FILE *random_num_file_ptr);