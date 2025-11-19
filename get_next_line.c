/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:32:17 by demane            #+#    #+#             */
/*   Updated: 2025/11/19 17:52:50 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len_malloc;
	char	*str;
	char	*ptr;

	if (!s1 || !s2)
		return (NULL);
	len_malloc = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len_malloc + 1));
	if (!str)
		return (NULL);
	ptr = &str[0];
	while (*(s1))
		*(str++) = *(s1++);
	while (*s2)
		*(str++) = *(s2++);
	*str = '\0';
	return (ptr);
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

void	ft_reintialise(char *return_value, t_line **l)
{
	int	i;

	i = 0;
	while (return_value[i] && return_value[i] != '\n')
		i++;
	if (return_value[i] == '\n' && return_value[i + 1])
		(*l)->line = (*l)->line + i + 1;
	else
	{
		free((*l)->line);
		(*l)->line = "";
	}
}

char	*ft_return_line(t_line *l)
{
	char	*r_str;
	char	*str;
	int		i;
	int		n;

	i = 0;
	if (l || l->line)
		return (NULL);
	str = l->line;
	while (str[i] && str[i] != '\n')
		i++;
	n = 2;
	if (!str[i])
		n = 1;
	r_str = (char *)malloc(sizeof(char) * (i + n));
	if (!r_str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		r_str[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		r_str[i++] = '\n';
	r_str[i] = '\0';
	ft_reintialise(r_str, &l);
	return (r_str);
}

void	ft_recup_next_line(t_line *l)
{
	size_t			size_read;
	char			*buffer;
	char			*tmp;

	size_read = BUFFER_SIZE;
	buffer = NULL;
	while (size_read == BUFFER_SIZE && !ft_include_new_line(buffer))
	{
		buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		size_read = read(l->fd, buffer, BUFFER_SIZE);
		tmp = ft_strjoin(l->line, buffer);
		free(l->line);
		free(buffer);
		l->line = tmp;
	}
}

t_line	*ft_init_line(t_line **l, int fd)
{
	t_line	*tmp;
	t_line	*prev;

	tmp = *l;
	if (!tmp)
		(*l) = (t_line *)malloc(sizeof(t_line) * 1);
	else
	{
		while (tmp)
		{
			if (tmp->fd == fd)
				return (tmp);
			prev = tmp;
			tmp = tmp->next;
		}
		tmp = (t_line *)malloc(sizeof(t_line) * 1);
		prev->next = tmp;
	}
	if (!tmp)
		tmp = (*l);
	tmp->fd = fd;
	tmp->line = NULL;
	tmp->next = NULL;
	return (tmp);
}

char	*get_next_line(int fd)
{
	static t_line	*l = NULL;
	t_line			*current;



	current = ft_init_line(&l, fd);
	ft_recup_next_line(current);
	return (ft_return_line(current));
}


