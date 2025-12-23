/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:32:11 by demane            #+#    #+#             */
/*   Updated: 2025/12/23 15:02:50 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include "unistd.h"
# include "stdlib.h"

typedef struct s_line
{
	char			*line;
	int				fd;
	struct s_line	*next;
}	t_line;

char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char *s1, char const *s2, int *lastlen);
char	*ft_return_line(t_line *l);
int		ft_recup_next_line(t_line *l);
t_line	*ft_init_line(t_line *l, int fd);
ssize_t	ft_strlen_gnl(const char *str);
int		ft_initialize_join(char **s1, char const **s2, int *i, int len[2]);
int		ft_include_new_line(char *str);
int		ft_reintialise(t_line **l);
void	ft_supp_maillon(t_line *l, int fd, t_line init);

#endif
