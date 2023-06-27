/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:28:28 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 15:42:27 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	*ft_hash_str(t_hash *data)
{
	t_lst	*value;
	char	*target;
	int		i;

	i = 0;
	while (data->key[i])
		i++;
	i += ft_str_size(data->value);
	target = ft_calloc(i + 2);
	i = 0;
	while (data->key[i])
	{
		target[i] = data->key[i];
		i++;
	}
	target[i++] = '=';
	value = data->value;
	while (value != NULL)
	{
		target[i++] = *(char *)value->data;
		value = value->nxt;
	}
	return (target);
}

char	*ft_substr(char *src, int n)
{
	char	*target;
	int		i;

	if (n == -1)
	{
		i = 0;
		while (src[i])
			i++;
		n = i;
	}
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
	int	i;

	write(2, "Error: ", 7);
	i = 0;
	while (msg[i])
		i++;
	write(2, msg, i);
	write(2, "\n", 1);
	g_status = 1;
	if (ft_strcmp((char *)msg, F_ERROR_EXE) == 0)
		return (127);
	if (ft_strcmp((char *)msg, F_ERROR_ACCESS) == 0)
		return (126);
	if (ft_strcmp((char *)msg, F_ERROR_EXPORT) == 0)
		return (-1);
	return (1);
}
