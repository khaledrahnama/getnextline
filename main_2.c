#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int     fd;
    char    *line;
    int     line_count;
    char    *filename;

    // If a filename is provided as argument, use it
    if (argc > 1)
        filename = argv[1];
    else
        filename = "test.txt";

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("Error: Could not open '%s'\n", filename);
        printf("Usage: ./gnl [filename]\n");
        printf("Or create a file called 'test.txt'\n");
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