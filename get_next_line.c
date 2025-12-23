/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:32:17 by demane            #+#    #+#             */
/*   Updated: 2025/12/23 15:02:04 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin_gnl(char *s1, char const *s2, int *lastlen)
{
	char	*str;
	char	*ptr;
	int		i;
	int		len[2];

	if (ft_initialize_join(&s1, &s2, &i, len))
		return (NULL);
	if ((len[0] + len[1]) >= *lastlen || *lastlen == 0)
	{
		str = (char *)malloc(sizeof(char) * ((len[0] + len[1]) * 2 + 1));
		if (!str)
			return (free(s1), NULL);
		while (s1 && s1[++i])
			str[i] = s1[i];
		i = -1;
		*lastlen = ((len[0] + len[1])) * 2 + 1;
		free(s1);
	}
	else
		str = s1;
	ptr = &str[0];
	while (s2[++i])
		str[len[0] + i] = s2[i];
	return (str[len[0] + len[1]] = '\0', ptr);
}

char	*ft_return_line(t_line *l)
{
	char	*r_str;
	char	*str;
	int		i;
	int		n;

	i = 0;
	if (!l || !l->line || l->line[0] == '\0')
		return (NULL);
	str = l->line;
	while (str[i] && str[i] != '\n')
		i++;
	n = 2;
	r_str = (char *)malloc(sizeof(char) * (i + n - !str[i]));
	if (!r_str)
		return (NULL);
	i = -1;
	while (str[++i] && str[i] != '\n')
		r_str[i] = str[i];
	if (str[i] == '\n')
		r_str[i++] = '\n';
	r_str[i] = '\0';
	if (ft_reintialise(&l))
		return (free(r_str), NULL);
	return (r_str);
}

int	ft_recup_next_line(t_line *l)
{
	int				size_read;
	char			*buffer;
	char			*tmp;
	int				lastlen;

	size_read = BUFFER_SIZE;
	lastlen = 0;
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (-1);
	*buffer = '\0';
	while (!ft_include_new_line(buffer) && size_read == BUFFER_SIZE)
	{
		size_read = read(l->fd, buffer, BUFFER_SIZE);
		if (size_read != -1)
		{
			buffer[size_read] = '\0';
			tmp = ft_strjoin_gnl(l->line, buffer, &lastlen);
			l->line = tmp;
			if (!tmp)
				return (free(buffer), -1);
		}
	}
	return (free(buffer), size_read);
}

t_line	*ft_init_line(t_line *l, int fd)
{
	t_line	*tmp;
	t_line	*prev;

	if (!l)
		tmp = l;
	else
	{
		tmp = l;
		while (tmp)
		{
			if (tmp->fd == fd)
				return (tmp);
			prev = tmp;
			tmp = tmp->next;
		}
		tmp = (t_line *)malloc(sizeof(t_line) * 1);
		if (!tmp)
			return (NULL);
		prev->next = tmp;
	}
	return ((tmp)->fd = fd, tmp->line = NULL, tmp->next = NULL, tmp);
}

char	*get_next_line(int fd)
{
	static t_line	l;
	t_line			*current;
	int				result_recup;
	char			*rv;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	current = ft_init_line(&l, fd);
	if (!current)
		return (NULL);
	result_recup = ft_recup_next_line(current);
	if (result_recup == -1)
	{
		ft_supp_maillon(&l, fd, l);
		return (NULL);
	}
	rv = ft_return_line(current);
	if (!rv)
		ft_supp_maillon(&l, fd, l);
	return (rv);
}
