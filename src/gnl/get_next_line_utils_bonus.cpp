/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-sous <bde-sous@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 20:59:46 by bde-sous          #+#    #+#             */
/*   Updated: 2023/01/12 22:18:00 by bde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/gnl/get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	char	*str;

	str = (char *)s;
	while ((unsigned char)*str != (unsigned char)c)
	{
		if (*str == '\0')
			return (0);
		str++;
	}
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;

	if (!s1 || !s2)
		return (0);
	ptr = (char*)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!(ptr))
		return (0);
	ft_bzero(ptr, (ft_strlen(s1) + ft_strlen(s2)) + 1);
	ft_memcpy(ptr, s1, ft_strlen(s1));
	ft_memcpy(ptr + ft_strlen(s1), s2, ft_strlen(s2));
	return (ptr);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*ptr_src;
	unsigned char	*ptr_dst;

	if (!dst && !src)
		return (0);
	ptr_src = (unsigned char *)src;
	ptr_dst = (unsigned char *)dst;
	i = 0;
	while (i++ < n)
		*ptr_dst++ = *ptr_src++;
	return (dst);
}

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
		ptr[i++] = '\0';
}
