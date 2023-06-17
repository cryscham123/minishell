/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:28:28 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/16 18:56:34 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	*ft_substr(char *src, int n)
{
	char	*target;
	int		i;

	target = ft_calloc(n + 1);
	if (target == NULL)
		return (NULL);
	i = 0;
	while (*src && i < n)
	{
		target[i] = src[i];
		i++;
	}
	return (target);
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

void	*ft_calloc(int size)
{
	int				i;
	unsigned char	*target;

	target = malloc(size);
	if (target == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		target[i] = 0;
		i++;
	}
	return (target);
}

int	ft_error(const char *msg)
{
	printf("Error: %s\n", msg);
	g_status = 1;
	return (1);
}
