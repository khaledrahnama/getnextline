/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khaledrahnama <khaledrahnama@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:16:12 by khaledrahna       #+#    #+#             */
/*   Updated: 2026/07/16 12:01:22 by khaledrahna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    
    int fd;
    fd = open(argv[1], O_RDONLY);
    char *line;
    line = NULL;
    int i = 1;


    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return (1);
    }

      fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf("Error opening file\n");
        return (1);
    }
    
    line = get_next_line(fd);
    while(line)
    {
            printf("line %d = %s", i, line);
            free(line);
            i++;
            line = get_next_line(fd);

    }
    close(fd);
}