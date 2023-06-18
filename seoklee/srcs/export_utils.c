/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoklee <seoklee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 16:05:28 by seoklee           #+#    #+#             */
/*   Updated: 2023/06/18 20:52:39 by seoklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_env_order(t_lst *ev, int flag)
{
	int	n;

	n = 0;
	if (flag == 1)
	{
		while (ev != NULL)
		{
			ev->size = n++;
			ev = ev->nxt;
		}
	}
	else
	{
		while (ev != NULL)
		{
			ev->size = 0;
			ev = ev->nxt;
		}
	}
}

static void	sort_env(t_lst *ev)
{
	int		tmp;
	t_lst	*i;
	t_lst	*j;

	i = ev;
	while (i != NULL)
	{
		j = ev;
		while (j != NULL)
		{
			if ((ft_strcmp(((t_hash *)i->data)->key, \
				((t_hash *)j->data)->key) > 0 && i->size < j->size) \
				|| (ft_strcmp(((t_hash *)i->data)->key, \
				((t_hash *)j->data)->key) < 0 && i->size > j->size))
			{
				tmp = i->size;
				i->size = j->size;
				j->size = tmp;
				j = ev;
			}
			j = j->nxt;
		}
		i = i->nxt;
	}
}

int	print_export(t_lst *ev)
{
	int		n;
	t_lst	*val;
	t_lst	*tmp;

	set_env_order(ev, 1);
	sort_env(ev);
	n = -1;
	while (++n < ft_str_size(ev))
	{
		tmp = ev;
		while (tmp != NULL && tmp->size != n)
			tmp = tmp->nxt;
		printf("%d, declare -x %s=", tmp->size, ((t_hash *)tmp->data)->key);
		val = ((t_hash *)tmp->data)->value;
		while (val != NULL)
		{
			printf("%c", *(char *)val->data);
			val = val->nxt;
		}
		printf("\n");
	}
	set_env_order(ev, 0);
	return (0);
}
