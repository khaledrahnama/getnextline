/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khaledrahnama <khaledrahnama@student.42.fr> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 00:00:00 by khaledrahna       #+#    #+#             */
/*   Updated: 2026/07/20 00:00:00 by khaledrahna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int		fd_a;
	int		fd_b;
	char	*line;
	int		a_done;
	int		b_done;

	if (argc != 3)
	{
		printf("Usage: %s <file1> <file2>\n", argv[0]);
		return (1);
	}
	fd_a = open(argv[1], O_RDONLY);
	fd_b = open(argv[2], O_RDONLY);
	if (fd_a < 0 || fd_b < 0)
	{
		printf("Error opening file\n");
		return (1);
	}
	a_done = 0;
	b_done = 0;
	while (!a_done || !b_done)
	{
		if (!a_done)
		{
			line = get_next_line(fd_a);
			if (line)
			{
				printf("[%s] %s", argv[1], line);
				free(line);
			}
			else
				a_done = 1;
		}
		if (!b_done)
		{
			line = get_next_line(fd_b);
			if (line)
			{
				printf("[%s] %s", argv[2], line);
				free(line);
			}
			else
				b_done = 1;
		}
	}
	close(fd_a);
	close(fd_b);
	return (0);
}
