/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 03:05:44 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*lst_back(t_lst *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->nxt != NULL)
		lst = lst->nxt;
	return (lst);
}

void	lst_replace(t_lst *del_st, t_lst *del_en, t_lst *re_st, t_lst *re_en)
{
	if (to_del->prev != NULL)
	{
		if (to_replace != NULL)
			to_del->prev->nxt = to_replace;
		else
			to_del->prev->nxt = to_del->nxt;
		to_del->prev = NULL;
	}
	if (to_del->nxt != NULL)
	{
		if(to_replace_en != NULL)
			to_del->nxt->prev = to_replace_en;
		else
			to_del->nxt->prev = to_del->prev;
		to_del->nxt = NULL;
	}
	ft_lst_free(to_del);
}
