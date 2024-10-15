#include "scheduler.h"

int main(int argc, char *argv[])
{
    // Check for correct number of arguments
    if (argc != 2)
    {
        printf("Error: Usage: ./scheduler <input-file>\n");
        return 1;
    }

    // Open the file
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    return 0;
}