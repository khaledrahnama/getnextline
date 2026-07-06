/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 13:00:42 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/06 21:02:57 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

# include <stdlib.h>
# include <unistd.h>

char *get_next_line(int fd);
char *ft_strjoin(char const *s1, char const *s2);
size_t ft_strlen(const char *s);
char *ft_strdup(const char *s);
char *ft_strchr(const char *s, int c);
char *ft_substr(char const *s, unsigned int start, size_t len); 


#endif