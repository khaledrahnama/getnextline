/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 13:00:35 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/21 17:00:00 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len])
	{
		len++;
	}
	return (len);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	string_len;
	char	*substring;

	i = 0;
	if (!s)
		return (NULL);
	string_len = ft_strlen(s);
	if (start >= string_len)
		return (ft_strdup(""));
	if (len > string_len - start)
		len = string_len - start;
	substring = malloc(len + 1);
	if (!substring)
		return (NULL);
	while (i < len && s[start + i])
	{
		substring[i] = s[start + i];
		i++;
	}
	substring[i] = '\0';
	return (substring);
}

int	grow_stash(t_stash *s, size_t needed_len)
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
