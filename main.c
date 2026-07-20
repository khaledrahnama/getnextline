/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:16:12 by khaledrahna       #+#    #+#             */
/*   Updated: 2026/07/20 21:28:43 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int fd;
    char *line;
    int i = 1;

    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return (1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return (1);
    }
    
    printf("BUFFER_SIZE = %d\n", BUFFER_SIZE); // Debug line
    
    line = get_next_line(fd);
    while (line)
    {
        printf("line %d = %s\n", i, line);
      //  free(line);
        i++;
        line = get_next_line(fd);
    }
    
    // Check if we reached EOF or if there was an error
    if (line == NULL)
        printf("Reached EOF or error at line %d\n", i);
    
    close(fd);
    return (0);
}