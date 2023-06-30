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

t_lst	*mk_ev(char **env)
{
	t_lst	*ev;
	t_lst	*to_push;

	ev = NULL;
	to_push = mk_lst("=$", F_DATA_CHAR, 0);
	if (to_push == NULL)
		return (NULL);
	lst_push(&ev, to_push);
	while (*env)
	{
		to_push = mk_lst(*env, F_DATA_CHAR, ft_str_find(*env, '='));
		if (to_push == NULL)
			return (lst_free(ev));
		lst_push(&ev, to_push);
		env++;
	}
	return (ev);
}

t_lst	*ft_env_find(t_lst *ev, char *s)
{
	while (ev != NULL)
	{
		((char *)ev->data)[ev->info] = '\0';
		if (ft_strcmp(ev->data, s) == 0)
		{
			((char *)ev->data)[ev->info] = '=';
			return (ev);
		}
		((char *)ev->data)[ev->info] = '=';
		ev = ev->nxt;
	}
	return (NULL);
}
