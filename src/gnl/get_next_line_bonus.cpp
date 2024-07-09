/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-sous <bde-sous@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:13:54 by bde-sous          #+#    #+#             */
/*   Updated: 2023/01/13 19:38:43 by bde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/gnl/get_next_line_bonus.h"

char	*ft_strdup(const char *s)
{
	char	*ptr;

	ptr = (char*)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (ptr == NULL)
		return (ptr);
	ft_bzero(ptr, ft_strlen(s) + 1);
	ft_memcpy(ptr, s, ft_strlen(s) + 1);
	return (ptr);
}

char	*ft_check_line(char *str)
{
	char	*newline;
	char	*result;
	size_t	len;

	if (!str)
		return (NULL);
	newline = ft_strchr(str, '\n');
	if (newline)
		len = newline - str + 1;
	else
		len = ft_strlen(str) + 1;
	result = (char*)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	ft_bzero(result, len + 1);
	ft_memcpy(result, str, len);
	return (result);
}

char	*ft_check_left(char *str)
{
	char	*newline;
	char	*result;

	if (!str || !str[0])
	{
		free(str);
		str = 0;
		return (NULL);
	}
	newline = ft_strchr(str, '\n');
	if (newline)
		result = ft_strdup(newline + 1);
	else
		result = 0;
	free(str);
	if (result && result[0] != '\0')
		return (result);
	if (result)
	{
		free(result);
		result = 0;
	}
	return (0);
}

char	*ft_read(char *s1, char *s2, int fd)
{
	char	*tmp;

	s2 = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!s2)
		return (0);
	ft_bzero(s2, BUFFER_SIZE + 1);
	while (recv(fd, s2, BUFFER_SIZE, 0) > 0)
	{
		if (!s1)
			s1 = ft_strdup(s2);
		else
		{
			tmp = ft_strdup(s1);
			free(s1);
			s1 = ft_strjoin(tmp, s2);
			free(tmp);
		}
		ft_bzero(s2, BUFFER_SIZE + 1);
		if (ft_strchr(s1, '\n'))
			break ;
	}
	free(s2);
	return (s1);
}

int	get_next_line(int fd, char **ret)
{
	static char	*pos[MAX_FD];
	char		*buffer;
	//char		*ret;
	char		*tmp;

	buffer = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (-1);
	pos[fd] = ft_read(pos[fd], buffer, fd);
	if (!pos[fd])
		return (0);
	*ret = ft_check_line(pos[fd]);
	tmp = ft_strdup(pos[fd]);
	free(pos[fd]);
	pos[fd] = ft_check_left(tmp);
	if (pos[fd] || *ret[0] != '\0')
		return (ft_strlen(*ret));
	free(pos[fd]);
	pos[fd] = 0;
	if (*ret[0] != '\0')
		return (ft_strlen(*ret));
	free(ret);
	return (0);
}
