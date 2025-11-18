/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:32:17 by demane            #+#    #+#             */
/*   Updated: 2025/11/18 17:07:43 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_include_new_line(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == "\n")
			return (1);
	}
	return (0);
}

int	ft_strlcat(char *s1, char *s2, size_t size)
{

}

char	*ft_read_fd(t_line *l)
{
	size_t			size_read;
	char			*buffer;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	while (size_read == BUFFER_SIZE && !ft_include_new_line(buffer))
	{
		size_read = read(l->fd, buffer, BUFFER_SIZE);
		if (size_read == -1)
			return (NULL);
		ft_strlcat(l->line, buffer, BUFFER_SIZE);
	}


}

char	*get_next_line(int fd)
{
	static t_line	*l = NULL;
	t_line			current;



	current = ft_init_line(l, fd);
	while (!ft_include_new_line(l->line))
	{
		current.line = ft_read_fd(current);

	}
}

t_line	ft_init_line(t_line *l, int fd)
{
	int	i;

	i = 0;
	if (!l)
	{
		l = (t_line *)malloc(sizeof(t_line) *1);
		l->fd = fd;
		l->next = NULL;
	}
	else
	{
		while (l)
		{
			if (l->fd == fd)
				return (*l);
			l = l->next;
		}
		l = (t_line *)malloc(sizeof(t_line) *1);
		l->fd = fd;
		l->next = NULL;
	}
	return (*l);
}
