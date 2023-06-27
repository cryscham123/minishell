/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 12:06:35 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:16 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_str_size(t_lst *str)
{
	int	i;

	i = 0;
	while (str != NULL)
	{
		str = str->nxt;
		i++;
	}
	return (i);
}

char	*ft_c_str(t_lst *str, t_lst *apd, char c, int n)
{
	char	*target;
	int		size;
	int		i;

	i = 0;
	if (n == -1)
		n = ft_str_size(str);
	size = n + ft_str_size(apd) + 1;
	target = ft_calloc(size + 1);
	if (target == NULL)
		return (NULL);
	while (str != NULL && i < n)
	{
		if (*(char *)str->data != '\0')
			target[i++] = *(char *)str->data;
		str = str->nxt;
	}
	target[i++] = c;
	while (apd != NULL && i < size)
	{
		target[i++] = *(char *)apd->data;
		apd = apd->nxt;
	}
	return (target);
}

int	dup_str_lst(t_lst **lst, t_lst *to_dup)
{
	char	*tmp;
	t_lst	*target;

	tmp = ft_c_str(to_dup, NULL, 0, -1);
	if (tmp == NULL)
		return (1);
	target = mk_str_lst(tmp);
	free(tmp);
	if (target == NULL)
		return (1);
	(*lst) = target;
	return (0);
}

t_lst	*ft_hash_find(t_lst *hash, char *to_find)
{
	t_hash	*cmp;

	while (hash != NULL)
	{
		cmp = hash->data;
		if (ft_strcmp(cmp->key, to_find) == 0)
			return (cmp->value);
		hash = hash->nxt;
	}
	return (NULL);
}

char	*ft_itoa(int a)
{
	char	*target;

	target = ft_calloc(4);
	if (target == NULL)
		return (NULL);
	target[0 + (a >= 10) + (a >= 100)] = "0123456789"[a % 10];
	a /= 10;
	if (a > 0)
		target[0 + (a >= 10)] = "0123456789"[a % 10];
	a /= 10;
	if (a > 0)
		target[0] = "0123456789"[a % 10];
	return (target);
}
