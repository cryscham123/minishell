/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:28:28 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/11 15:33:06 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_strncpy(char *dst, char *src, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
}
