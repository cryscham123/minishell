/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:35:44 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	ft_str_find(char *s, char to_find)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == to_find)
			return (i);
		i++;
	}
	return (-1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_substr(char *s, int n)
{
	char	*target;
	int		i;

	if (n == -1)
		n = ft_strlen(s);
	target = ft_calloc(n + 1);
	if (target == NULL)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		target[i] = s[i];
		i++;
	}
	return (target);
}

char	*ft_append(char *s1, char *s2, char middle)
{
	char	*target;
	int		s1_len;
	int		s2_len;
	int		i;
	int		j;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	target = ft_calloc(s1_len + s2_len + (middle != 0) + 1);
	if (target == NULL)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		target[i] = s1[i];
	if (middle != 0)
		target[i++] = middle;
	j = -1;
	while (++j < s2_len)
		target[i + j] = s2[j];
	return (target);
}
