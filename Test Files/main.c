#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int     fd;
    char    *line;
    int     line_count;
    char    *filename;

    // Check if a filename was provided as argument
    if (argc < 2)
    {
        printf("Usage: ./gnl <filename>\n");
        printf("Example: ./gnl test.txt\n");
        return (1);
    }

    filename = argv[1];
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("Error: Could not open file '%s'\n", filename);
        printf("Make sure the file exists and is readable.\n");
        return (1);
    }

    printf("=== Reading from: %s ===\n", filename);
    line_count = 1;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", line_count, line);
        free(line);
        line_count++;
    }
    printf("Total lines read: %d\n", line_count - 1);
    close(fd);
    return (0);
}