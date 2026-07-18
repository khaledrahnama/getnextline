/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khaledrahnama <khaledrahnama@student.42.fr> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 00:00:00 by khaledrahna       #+#    #+#             */
/*   Updated: 2026/07/18 00:00:00 by khaledrahna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*read_and_append(int fd, char **stash)
{
	char	*buffer;
	char	*temp;
	ssize_t	bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(*stash, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(*stash);
			*stash = NULL;
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(*stash, buffer);
		free(*stash);
		*stash = temp;
		if (!*stash)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (*stash);
}

static char	*extract_line(char **stash)
{
	char	*line;
	char	*newline_pos;
	char	*temp;
	size_t	line_len;

	if (!*stash || !**stash)
		return (NULL);
	newline_pos = ft_strchr(*stash, '\n');
	if (newline_pos)
	{
		line_len = newline_pos - *stash + 1;
		line = ft_substr(*stash, 0, line_len);
		if (!line)
			return (NULL);
		temp = ft_strdup(newline_pos + 1);
		if (!temp)
		{
			free(line);
			free(*stash);
			*stash = NULL;
			return (NULL);
		}
		free(*stash);
		*stash = temp;
	}
	else
	{
		line = ft_strdup(*stash);
		free(*stash);
		*stash = NULL;
		if (!line)
			return (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*line;

	line = NULL;
	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash[fd])
	{
		stash[fd] = ft_strdup("");
		if (!stash[fd])
			return (NULL);
	}
	if (!read_and_append(fd, &stash[fd]))
		return (NULL);
	if (!stash[fd] || !*stash[fd])
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	line = extract_line(&stash[fd]);
	if (!line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	return (line);
}
