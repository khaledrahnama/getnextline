/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 13:00:35 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/03 14:15:18 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t ft_strlen(const char *s)
{
    size_t len = 0;

    if(!s)
        return 0;
        
    while(s[len])
    {
        len++;
    }
    return len;
}

char *ft_strdup(const char *s)
{
    size_t len;
    size_t i;
    char *dup;
    i = 0;

    if(!s)
        return NULL; 
    
    len = ft_strlen(s);

    dup = malloc(len + 1);
    if(!dup)
        return NULL;
        
    while(s[i])
    {
        dup[i] = s[i];
        i++;
    }

    dup[i] = '\0';
    return (dup);
}

