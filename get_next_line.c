/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 13:00:38 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/06 21:18:57 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


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
		free(*stash);
		*stash = temp;
	}
	else
	{
		line = ft_strdup(*stash);
		free(*stash);
		*stash  = NULL;
		if (!line)
			return (NULL);
	}
	return (line);
}


char *get_next_line(int fd)
{
    static char *stash = NULL; // Initialize buffer to NULL for the first call
    char *line;
    line = NULL; // Initialize line to NULL for the first call

    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;
    
    if(!stash)
    {
       stash = ft_strdup(""); // Initialize buffer to an empty string if it's NULL
       if(!stash)
           return NULL; // Handle memory allocation failure
    }
    if (!read_and_append(fd, &stash))
		return (NULL);
	if (!stash || !*stash   )
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = extract_line(&stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	return (line);

}
