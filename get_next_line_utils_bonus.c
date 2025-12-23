/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:32:14 by demane            #+#    #+#             */
/*   Updated: 2025/12/23 15:04:25 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

ssize_t	ft_strlen_gnl(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	ft_initialize_join(char **s1, char const **s2,
		int *i, int len[2])
{
	if (!*s1 && !*s2)
		return (1);
	if (!*s2)
		*s2 = "";
	*i = -1;
	len[0] = ft_strlen_gnl(*s1);
	len[1] = ft_strlen_gnl(*s2);
	return (0);
}

int	ft_include_new_line(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i++] == '\n')
			return (1);
	}
	return (0);
}

int	ft_reintialise(t_line **l)
{
	int		i;
	int		j;
	char	*lastvalue;
	char	*str;

	str = (*l)->line;
	if (ft_include_new_line(str))
	{
		j = 0;
		while (str[j] != '\n')
			j++;
		lastvalue = (*l)->line;
		(*l)->line = (char *)malloc(sizeof(char)
				* (ft_strlen_gnl(str) - j + 1));
		if ((*l)->line == NULL)
			return (free(lastvalue), 1);
		i = 0;
		while (str[++j])
			(*l)->line[i++] = str[j];
		if (i == 0)
			return (free((*l)->line), (*l)->line = NULL, free(lastvalue), 0);
		return ((*l)->line[i] = '\0', free(lastvalue), 0);
	}
	else
		return (free((*l)->line), (*l)->line = NULL, 0);
}

void	ft_supp_maillon(t_line *l, int fd, t_line init)
{
	t_line	*prev;
	t_line	*next;
	t_line	*current;

	prev = NULL;
	current = l;
	while (current)
	{
		next = current->next;
		if (current->fd == fd)
		{
			if (prev)
				prev->next = next;
			else
				l = next;
			free(current->line);
			if (current->fd != init.fd)
				free(current);
			current = NULL;
		}
		if (current)
			prev = current;
		current = next;
	}
}
