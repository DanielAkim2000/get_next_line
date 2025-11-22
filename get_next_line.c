/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:32:17 by demane            #+#    #+#             */
/*   Updated: 2025/11/22 16:14:50 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char const *s2, int *lastlen)
{
	char	*str;
	char	*ptr;
	int		i;
	int		len_s2;
	int		len_s1;

	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		s2 = "";
	i = -1;
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if ((len_s1 + len_s2) >= *lastlen || *lastlen == 0)
	{
		str = (char *)malloc(sizeof(char) * ((len_s1 + len_s2) * 2 + 1));
		if (!str)
			return (s1 ? free(s1) : (void)0, NULL);
		ptr = &str[0];
		while (s1 && s1[++i])
			str[i]= s1[i];
		i = -1;
		while (s2[++i])
			str[len_s1 + i] = s2[i];
		*lastlen = ((len_s1 + len_s2)) * 2 + 1;
		s1 ? free(s1): NULL;
	}
	else
	{
		str = s1;
		ptr = &str[0];
		while (s2[++i])
			str[len_s1 + i] = s2[i];
	}
	str[len_s1 + len_s2] = '\0';
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

void	ft_supp_maillon(t_line **l, int fd)
{
	t_line	*prev;
	t_line	*next;
	t_line	*current;

	prev = NULL;
	next = NULL;
	if (*l)
	{
		current = *l;
		while (current)
		{
			next = current->next;
			if (current->fd == fd)
			{
				if (prev)
					prev->next = next;
				else
					*l = next;
				if (current->line)
					free(current->line);
				free(current);
				current = NULL;
			}
			if (current)
				prev = current;
			current = next;
		}
	}
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
		(*l)->line = (char *)malloc(sizeof(char) * (ft_strlen(str) - j + 1));
		if ((*l)->line == NULL)
		{
			free(lastvalue);
			return (1);
		}
		i = 0;
		while (str[++j])
			(*l)->line[i++] = str[j];
		(*l)->line[i] = '\0';
		free(lastvalue);
	}
	else
	{
		if ((*l)->line)
			free((*l)->line);
		(*l)->line = NULL;
	}
	return (0);
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
	if (ft_reintialise(&l))
		return (free(r_str), NULL);
	return (r_str);
}


int	ft_recup_next_line(t_line *l)
{
	int				size_read;
	// char			buffer[BUFFER_SIZE + 1];
	char			*buffer;
	char			*tmp;
	char			*line;
	int				lastlen;

	size_read = BUFFER_SIZE;
	lastlen = 0;
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (-1);
	*buffer = '\0';
	while (!ft_include_new_line(buffer) && size_read == BUFFER_SIZE
		&& size_read != -1)
	{
		size_read = read(l->fd, buffer, BUFFER_SIZE);
		if (size_read == -1)
		{
			if (l->line)
				free(l->line);
			l->line = NULL;
		}
		else
		{
			buffer[size_read] = '\0';
			line = l->line;
			tmp = ft_strjoin(line, buffer, &lastlen);
			l->line = tmp;
			if (!tmp)
				return (free(buffer), -1);
		}
	}
	if (buffer)
		free(buffer);
	return (size_read);
}

t_line	*ft_init_line(t_line **l, int fd)
{
	t_line	*tmp;
	t_line	*prev;

	tmp = *l;
	if (!tmp)
	{
		(*l) = (t_line *)malloc(sizeof(t_line) * 1);
		if (!*l)
			return (NULL);
	}
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
		if (!tmp)
			return (NULL);
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
		ft_supp_maillon(&l, fd);
		return (NULL);
	}
	rv = ft_return_line(current);
	if (!rv)
		ft_supp_maillon(&l, fd);
	return (rv);
}



// size_t	ft_strlen(const char *str)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!str)
// 		return (0);
// 	while (str[i])
// 		i++;
// 	return (i);
// }

// char	*ft_strjoin(char *s1, char const *s2, int *lastlen)
// {
// 	char	*str;
// 	char	*ptr;
// 	int		i;
// 	int		len_s2;
// 	int		len_s1;

// 	if (!s1 && !s2)
// 		return (NULL);
// 	if (!s2)
// 		s2 = "";
// 	i = -1;
// 	len_s1 = ft_strlen(s1);
// 	len_s2 = ft_strlen(s2);
// 	if ((len_s1 + len_s2) >= *lastlen || *lastlen == 0)
// 	{
// 		str = (char *)malloc(sizeof(char) * ((len_s1 + len_s2) * 2 + 1));
// 		if (!str)
// 			return (s1 ? free(s1) : (void)0, NULL);
// 		ptr = &str[0];
// 		while (s1 && s1[++i])
// 			str[i]= s1[i];
// 		i = -1;
// 		while (s2[++i])
// 			str[len_s1 + i] = s2[i];
// 		*lastlen = ((len_s1 + len_s2)) * 2 + 1;
// 		s1 ? free(s1): NULL;
// 	}
// 	else
// 	{
// 		str = s1;
// 		ptr = &str[0];
// 		while (s2[++i])
// 			str[len_s1 + i] = s2[i];
// 	}
// 	str[len_s1 + len_s2] = '\0';
// 	return (ptr);
// }

// int	ft_include_new_line(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (!str)
// 		return (0);
// 	while (str[i])
// 	{
// 		if (str[i++] == '\n')
// 			return (1);
// 	}
// 	return (0);
// }

// void	ft_supp_maillon(t_line **l, int fd)
// {
// 	t_line	*prev;
// 	t_line	*next;
// 	t_line	*current;

// 	prev = NULL;
// 	next = NULL;
// 	if (*l)
// 	{
// 		current = *l;
// 		while (current)
// 		{
// 			next = current->next;
// 			if (current->fd == fd)
// 			{
// 				if (prev)
// 					prev->next = next;
// 				else
// 					*l = next;
// 				if (current->line)
// 					free(current->line);
// 				free(current);
// 				current = NULL;
// 			}
// 			if (current)
// 				prev = current;
// 			current = next;
// 		}
// 	}
// }


// int	ft_reintialise(t_line **l)
// {
// 	int		i;
// 	int		j;
// 	char	*lastvalue;
// 	char	*str;

// 	str = (*l)->line;
// 	if (ft_include_new_line(str))
// 	{
// 		j = 0;
// 		while (str[j] != '\n')
// 			j++;
// 		lastvalue = (*l)->line;
// 		(*l)->line = (char *)malloc(sizeof(char) * (ft_strlen(str) - j + 1));
// 		if ((*l)->line == NULL)
// 		{
// 			free(lastvalue);
// 			return (1);
// 		}
// 		i = 0;
// 		while (str[++j])
// 			(*l)->line[i++] = str[j];
// 		(*l)->line[i] = '\0';
// 		free(lastvalue);
// 	}
// 	else
// 	{
// 		if ((*l)->line)
// 			free((*l)->line);
// 		(*l)->line = NULL;
// 	}
// 	return (0);
// }

// char	*ft_return_line(t_line *l)
// {
// 	char	*r_str;
// 	char	*str;
// 	int		i;
// 	int		n;

// 	i = 0;
// 	if (!l || !l->line || l->line[0] == '\0')
// 		return (NULL);
// 	str = l->line;
// 	while (str[i] && str[i] != '\n')
// 		i++;
// 	n = 2;
// 	if (!str[i])
// 		n = 1;
// 	r_str = (char *)malloc(sizeof(char) * (i + n));
// 	if (!r_str)
// 		return (NULL);
// 	i = 0;
// 	while (str[i] && str[i] != '\n')
// 	{
// 		r_str[i] = str[i];
// 		i++;
// 	}
// 	if (str[i] == '\n')
// 		r_str[i++] = '\n';
// 	r_str[i] = '\0';
// 	if (ft_reintialise(&l))
// 	{
// 		free(r_str);
// 		return (NULL);
// 	}
// 	return (r_str);
// }


// int	ft_recup_next_line(t_line *l)
// {
// 	int				size_read;
// 	char			buffer[BUFFER_SIZE + 1];
// 	char			*tmp;
// 	char			*line;
// 	int				lastlen;

// 	size_read = BUFFER_SIZE;
// 	lastlen = 0;
// 	buffer[0] = '\0';
// 	while (!ft_include_new_line(buffer) && size_read == BUFFER_SIZE
// 		&& size_read != -1)
// 	{
// 		size_read = read(l->fd, buffer, BUFFER_SIZE);
// 		if (size_read == -1)
// 		{
// 			if (l->line)
// 				free(l->line);
// 			l->line = NULL;
// 		}
// 		else
// 		{
// 			buffer[size_read] = '\0';
// 			line = l->line;
// 			tmp = ft_strjoin(line, buffer, &lastlen);
// 			l->line = tmp;
// 			if (!tmp)
// 				return (-1);
// 		}
// 	}
// 	return (size_read);
// }

// t_line	*ft_init_line(t_line **l, int fd)
// {
// 	t_line	*tmp;
// 	t_line	*prev;

// 	tmp = *l;
// 	if (!tmp)
// 	{
// 		(*l) = (t_line *)malloc(sizeof(t_line) * 1);
// 		if (!*l)
// 			return (NULL);
// 	}
// 	else
// 	{
// 		while (tmp)
// 		{
// 			if (tmp->fd == fd)
// 				return (tmp);
// 			prev = tmp;
// 			tmp = tmp->next;
// 		}
// 		tmp = (t_line *)malloc(sizeof(t_line) * 1);
// 		if (!tmp)
// 			return (NULL);
// 		prev->next = tmp;
// 	}
// 	if (!tmp)
// 		tmp = (*l);
// 	tmp->fd = fd;
// 	tmp->line = NULL;
// 	tmp->next = NULL;
// 	return (tmp);
// }

// char	*get_next_line(int fd)
// {
// 	static t_line	*l = NULL;
// 	t_line			*current;
// 	int				result_recup;
// 	char			*rv;


// 	if (fd < 0 || BUFFER_SIZE < 1)
// 		return (NULL);
// 	current = ft_init_line(&l, fd);
// 	if (!current)
// 		return (NULL);
// 	result_recup = ft_recup_next_line(current);
// 	if (result_recup == -1)
// 	{
// 		ft_supp_maillon(&l, fd);
// 		return (NULL);
// 	}
// 	rv = ft_return_line(current);
// 	if (!rv)
// 		ft_supp_maillon(&l, fd);
// 	return (rv);
// }


// int main(void)
// {
// 	char	*nl;

// 	int fd = open("file.txt", O_RDONLY);
// 	nl = get_next_line(fd);
// 	printf("\nNext_line:%s", nl);
// 	free(nl);
// 	nl = get_next_line(fd);
// 	printf("\nNext_line:%s", nl);
// 	free(nl);
// 	close(fd);
// }


