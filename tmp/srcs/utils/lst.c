/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 00:18:50 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lst_size(t_lst *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		i++;
		lst = lst->nxt;
	}
	return (i);
}

t_lst	*mk_lst(void *data, int data_type, int info)
{
	t_lst	*target;

	target = ft_calloc(sizeof(t_lst));
	if (target == NULL)
		return (NULL);
	target->data = data;
	target->data_type = data_type;
	target->info = info;
	return (target);
}

void	lst_push(t_lst **target, t_lst *to_push)
{
	t_lst	*tmp;

	tmp = *target;
	if (tmp == NULL)
	{
		*target = to_push;
		return ;
	}
	while (tmp->nxt != NULL)
		tmp = tmp->nxt;
	tmp->nxt = to_push;
	to_push->prev = tmp;
}

void	lst_delete(t_lst *to_del)
{
	if (to_del == NULL)
		return ;
	if (to_del->prev != NULL)
		to_del->prev->nxt = to_del->nxt;
	if (to_del->nxt != NULL)
		to_del->nxt->prev = to_del->prev;
	to_del->nxt = NULL;
	ft_lst_free(to_del);
}
