/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:32:11 by demane            #+#    #+#             */
/*   Updated: 2025/11/18 15:13:59 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2
# endif

# include "unistd.h"
# include "stdlib.h"

typedef struct s_line{
	char	*line;
	int		fd;
	s_line	*next;
}	t_line;

char	*get_next_line(int fd);

#endif
