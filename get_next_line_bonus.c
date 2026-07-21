/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khaledrahnama <khaledrahnama@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 00:00:00 by khaledrahna       #+#    #+#             */
/*   Updated: 2026/07/21 13:54:36 by khaledrahna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

typedef struct s_stash
{
	char	*data;
	size_t	len;
	size_t	capacity;
}	t_stash;

static int	grow_stash(t_stash *s, size_t needed_len)
{
	char	*bigger;
	size_t	new_capacity;
	size_t	i;

	if (needed_len < s->capacity)
		return (1);
	new_capacity = s->capacity;
	if (new_capacity < 1)
		new_capacity = 1;
	while (new_capacity <= needed_len)
		new_capacity = new_capacity * 2;
	bigger = malloc(new_capacity);
	if (!bigger)
		return (0);
	i = 0;
	while (i < s->len)
	{
		bigger[i] = s->data[i];
		i++;
	}
	free(s->data);
	s->data = bigger;
	s->capacity = new_capacity;
	return (1);
}

static int	append_chunk(t_stash *s, char *buffer, size_t bytes)
{
	if (!grow_stash(s, s->len + bytes))
		return (0);
	ft_memcpy(s->data + s->len, buffer, bytes);
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

static void	clear_stash(t_stash *s)
{
	free(s->data);
	s->data = NULL;
}

static void	resync_len(t_stash *s)
{
	s->len = ft_strlen(s->data);
	s->capacity = s->len + 1;
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
		clear_stash(s);
		return (NULL);
	}
	clear_stash(s);
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
		clear_stash(s);
	}
	if (!line)
		return (NULL);
	resync_len(s);
	return (line);
}

static char	*reset_and_fail(t_stash *s)
{
	free(s->data);
	s->data = NULL;
	s->len = 0;
	s->capacity = 0;
	return (NULL);
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
		s[fd].capacity = 1;
	}
	if (!read_and_append(fd, &s[fd]) || !s[fd].data || !*s[fd].data)
		return (reset_and_fail(&s[fd]));
	line = extract_line(&s[fd]);
	if (!line)
		return (reset_and_fail(&s[fd]));
	return (line);
}
