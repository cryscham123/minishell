/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 02:13:14 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lst_find(t_lst *s, char c)
{
	while (s != NULL && *(char *)s->data != '\0')
	{
		if (*(char *)s->data == c)
			return (0);
		s = s->nxt;
	}
	return (-1);
}

char	*ft_c_str(t_lst *s)
{
	char	*ret;
	int		size;
	int		i;

	size = ft_lst_size(s);
	ret = ft_calloc(size + 1);
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (s != NULL)
	{
		if (s->info != F_NO_PARSE)
			ret[i++] = *(char *)s->data;
		s = s->nxt;
	}
	return (ret);
}

t_lst	*ft_lst_trim(t_lst *lst)
{
	t_lst	*tmp;

	while (lst != NULL && ft_str_find(" \t\n", *(char *)lst->data) != -1)
		lst = lst->nxt;
	tmp = lst_back(lst);
	while (tmp != lst)
	{
		if (tmp == NULL || ft_str_find(" \t\n", *(char *)tmp->data) == -1)
			break ;
		*(char *)tmp->data = '\0';
		tmp = tmp->prev;
	}
	return (lst);
}

t_lst	*mk_str_lst(char *s)
{
	t_lst	*ret;
	t_lst	*to_push;

	ret = NULL;
	while (*s)
	{
		to_push = mk_str_node(s, 1, 0);
		if (to_push == NULL)
			return (ft_lst_free(ret));
		lst_push(&ret, to_push);
		s++;
	}
	return (ret);
}

t_lst	*str_lst_dup(t_lst *lst, int n)
{
	t_lst	*ret;
	t_lst	*to_push;
	int		i;

	i = 0;
	ret = NULL;
	while (lst != NULL && i < n)
	{
		to_push = mk_str_node(lst->data, 1, lst->info);
		if (to_push == NULL)
			return (ft_lst_free(ret));
		lst_push(&ret, to_push);
		lst = lst->nxt;
		i++;
	}
	return (ret);
}
