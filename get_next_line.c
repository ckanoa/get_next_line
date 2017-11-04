/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckanoa <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/16 11:42:45 by ckanoa            #+#    #+#             */
/*   Updated: 2017/08/09 15:38:10 by ckanoa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include "get_next_line.h"

static void	*ft_realloc(void *old, size_t size)
{
	void *new;

	if (old == NULL)
	{
		if (!(new = (void *)malloc((size))))
			return (NULL);
	}
	else if (size == 0)
	{
		if (!(new = (void *)malloc(1)))
			return (NULL);
	}
	else
	{
		if (!(new = (void *)malloc((size))))
			return (NULL);
		new = ft_memcpy(new, old, size);
		ft_memdel(&old);
	}
	return (new);
}

static int	read_fill(const int fd, size_t len, char **line, char **s)
{
	char	*tmp;
	size_t	count;
	int		buf;

	count = 0;
	tmp = ft_strnew(BUFF_SIZE);
	while ((buf = read(fd, tmp, BUFF_SIZE)) > 0)
	{
		count += buf;
		if (!(*line = (char*)ft_realloc((void*)(*line), count + 1 + len)))
			return (-1);
		ft_strncat(*line, tmp, buf);
		if (ft_strchr(tmp, '\n'))
			break ;
	}
	ft_memdel((void**)(&tmp));
	if ((buf == -1) || (buf == 0 && !(*line)))
		return (buf == -1 ? -1 : 0);
	if (ft_strchr(*line, '\n'))
	{
		*s = ft_strdup(ft_strchr(*line, '\n') + 1);
		*(ft_strchr(*line, '\n')) = 0;
	}
	return (1);
}

int			get_next_line(const int fd, char **line)
{
	static char	*s[FD_MAX];
	size_t		len;

	if (fd < 0 || fd >= FD_MAX || !(line))
		return (-1);
	if (*line)
		*line = NULL;
	len = 0;
	if (s[fd] != NULL && ft_strlen(s[fd]) > 0)
	{
		len = !(ft_strchr(s[fd], '\n')) ? ft_strlen(s[fd]) : ft_strchr(s[fd],
				'\n') + 1 - s[fd];
		*line = ft_strnew(len);
		ft_memccpy(*line, s[fd], '\n', ft_strlen(s[fd]));
		if (ft_strchr(*line, '\n'))
		{
			*(ft_strchr(*line, '\n')) = 0;
			ft_memmove(s[fd], ft_strchr(s[fd], '\n') + 1, ft_strlen(s[fd]));
			return (1);
		}
		ft_strdel(&s[fd]);
	}
	return (read_fill(fd, len, line, &s[fd]));
}
