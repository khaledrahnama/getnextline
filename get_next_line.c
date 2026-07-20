/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 13:00:38 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/21 00:32:39 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khaledrahnama <khaledrahnama@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 13:00:38 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/16 10:13:41 by khaledrahna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


static char	*grow_stash(char *stash, size_t used_len, size_t needed_len,
		size_t *capacity)
{
	char	*bigger;
	size_t	new_capacity;
	size_t	i;

	if (needed_len < *capacity)
		return (stash);
	new_capacity = *capacity;
	if (new_capacity < 1)
		new_capacity = 1;
	while (new_capacity <= needed_len)
		new_capacity = new_capacity * 2;
	bigger = malloc(new_capacity);
	if (!bigger)
		return (NULL);
	i = 0;
	while (i < used_len)
	{
		bigger[i] = stash[i];
		i++;
	}
	free(stash);
	*capacity = new_capacity;
	return (bigger);
}

static char	*read_and_append(int fd, char **stash)
{
	char	*buffer;
	ssize_t	bytes_read;
	size_t	len;
	size_t	capacity;
	size_t	i;

	if (ft_strchr(*stash, '\n'))
		return (*stash);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	len = ft_strlen(*stash);
	capacity = len + 1;
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(*stash);
			*stash = NULL;
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		*stash = grow_stash(*stash, len, len + (size_t)bytes_read, &capacity);
		if (!*stash)
		{
			free(buffer);
			return (NULL);
		}
		i = 0;
		while (i < (size_t)bytes_read)
		{
			(*stash)[len + i] = buffer[i];
			i++;
		}
		len = len + (size_t)bytes_read;
		(*stash)[len] = '\0';
		if (ft_strchr(buffer, '\n'))
			break ;
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
    static char *stash = NULL; 
    char *line;
    line = NULL; 
	
    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;
    
    if(!stash)
    {
       stash = ft_strdup(""); 
       if(!stash)
           return NULL; 
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