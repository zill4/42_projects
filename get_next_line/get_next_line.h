/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcrisp <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 11:16:48 by jcrisp            #+#    #+#             */
/*   Updated: 2018/11/27 11:17:35 by jcrisp           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include "./libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# define BUFF_SIZE 5

int		get_next_line(const int fd, char **line);
int		ft_stralloc(char **str, char **line, int fd, int val);

#endif
