/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 21:04:27 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/23 21:05:42 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	append_chunk(t_stash *s, char *buffer, size_t bytes)
{
	size_t	i;

	if (!grow_stash(s, s->len + bytes))
		return (0);
	i = 0;
	while (i < bytes)
	{
		s->data[s->len + i] = buffer[i];
		i++;
	}
	s->len += bytes;
	s->data[s->len] = '\0';
	return (1);
}

static int	read_and_append(int fd, t_stash *s)
{
	char	*buffer;
	ssize_t	bytes;

	if (ft_strchr(s->data, '\n'))
		return (1);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	bytes = read(fd, buffer, BUFFER_SIZE);
	while (bytes > 0)
	{
		buffer[bytes] = '\0';
		if (!append_chunk(s, buffer, (size_t)bytes))
		{
			free(buffer);
			return (0);
		}
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (bytes != -1);
}

static char	*split_at_newline(t_stash *s, char *newline_pos, size_t line_len)
{
	char	*line;
	char	*temp;

	line = ft_substr(s->data, 0, line_len);
	if (!line)
		return (NULL);
	temp = ft_strdup(newline_pos + 1);
	if (!temp)
	{
		free(line);
		free(s->data);
		s->data = NULL;
		return (NULL);
	}
	free(s->data);
	s->data = temp;
	return (line);
}

static char	*extract_line(t_stash *s)
{
	char	*line;
	char	*newline_pos;
	size_t	line_len;

	if (!s->data || !*s->data)
		return (NULL);
	newline_pos = ft_strchr(s->data, '\n');
	if (newline_pos)
	{
		line_len = newline_pos - s->data + 1;
		line = split_at_newline(s, newline_pos, line_len);
	}
	else
	{
		line = ft_strdup(s->data);
		free(s->data);
		s->data = NULL;
	}
	if (!line)
		return (NULL);
	s->len = ft_strlen(s->data);
	s->capacity = s->len + 1;
	return (line);
}

char	*get_next_line(int fd)
{
	static t_stash	s[MAX_FD];
	char			*line;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (!s[fd].data)
	{
		s[fd].data = ft_strdup("");
		if (!s[fd].data)
			return (NULL);
		s[fd].len = 0;
		s[fd].capacity = 1;
	}
	line = NULL;
	if (read_and_append(fd, &s[fd]) && s[fd].data && *s[fd].data)
		line = extract_line(&s[fd]);
	if (!line)
	{
		free(s[fd].data);
		s[fd].data = NULL;
	}
	return (line);
}
