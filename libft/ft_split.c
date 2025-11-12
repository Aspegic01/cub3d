/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:11:10 by mgamraou          #+#    #+#             */
/*   Updated: 2025/10/23 20:37:21 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	splitcount(char const *s, char c)
{
	int	i;
	int	count;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	if (s[0] != c && s[0] != '\0')
		count++;
	while (s[i])
	{
		if (s[i - 1] == c && s[i] != c)
			count++;
		i++;
	}
	return (count);
}

static void	needfree(char **strs, int i)
{
	int	j;

	j = 0;
	while (j <= i)
	{
		free(strs[j]);
		j++;
	}
	free(strs);
}

static int	countlen(const char *s, char c)
{
	int	len;

	if (!ft_strchr(s, c))
		len = ft_strlen(s);
	else
		len = ft_strchr(s, c) - s;
	return (len);
}

static int	checkdel(const char **s, char c)
{
	while (**s == c && *s)
		(*s)++;
	if (**s == '\0')
		return (0);
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		slen;
	char	**strs;

	if (!s)
		return (NULL);
	i = 0;
	strs = (char **)malloc(sizeof(char *) * (splitcount(s, c) + 1));
	if (!strs)
		return (NULL);
	while (*s)
	{
		if (checkdel(&s, c) == 0)
			break ;
		slen = countlen(s, c);
		strs[i++] = ft_substr(s, 0, slen);
		if (!strs[i - 1])
		{
			needfree(strs, i);
			return (NULL);
		}
		s += slen;
	}
	strs[i] = NULL;
	return (strs);
}
