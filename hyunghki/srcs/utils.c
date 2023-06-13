/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:28:28 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/13 11:59:32 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char *src, int n)
{
	char	*target;
	int		i;

	target = ft_calloc(n + 1);
	if (target == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		target[i] = src[i];
		i++;
	}
	return (target);
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
