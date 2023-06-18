/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoklee <seoklee@student.42.kr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 00:06:06 by seoklee           #+#    #+#             */
/*   Updated: 2023/06/18 01:34:57 by seoklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_lst(t_lst **ev, t_lst *target)
{
	if (target == *ev)
	{
		*ev = (*ev)->nxt;
		target->nxt = NULL;
		
	}
}

void	builtin_unset(char **argv, t_lst *ev)
{
	int		i;
	char	*str;
	t_lst	*tmp;
	t_lst	*free_ev;

	i = 0;
	while (argv[++i])
	{
		tmp = ev;
		while (tmp != NULL)
		{
			if (!ft_strcmp(tmp->data->key, argv[i]))
				remove_lst(&ev, tmp);
			tmp = tmp->nxt;
		}
	}
}