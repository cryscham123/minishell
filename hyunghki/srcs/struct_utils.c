/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 12:06:35 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/17 18:01:42 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_str_size(t_lst *str)
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

char	*ft_c_str(t_lst *str, int n, int del_quote)
{
	char	*target;
	char	c;
	int		i;

	i = 0;
	if (n == -1)
		n = ft_str_size(str);
	target = ft_calloc(n + 1);
	if (target == NULL)
		return (NULL);
	while (str != NULL && i < n)
	{
		c = *(char *)str->data;
		if (!(del_quote && (c == '\"' || c == '\'')))
			target[i++] = c;
		str = str->nxt;
	}
	target[i] = '\0';
	return (target);
}

int	dup_str_lst(t_lst **lst, t_lst *to_dup)
{
	char	*tmp;
	t_lst	*target;

	tmp = ft_c_str(to_dup, to_dup->size, 0);
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

t_lst	*ft_itoa(int a)
{
	t_lst	*target;

	target = ft_calloc(sizeof(t_lst));
	if (target == NULL)
		return (NULL);
	target->data = ft_substr(&"0123456789"[a % 10], 1);
	if (target->data == NULL)
		return (ft_lst_free(target, f_data_char, NULL));
	if (a < 10)
		return (target);
	target->nxt = ft_itoa(a / 10);
	if (target->nxt == NULL)
		return (ft_lst_free(target, f_data_char, NULL));
	return (target);
}
