/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoklee <seoklee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 16:05:28 by seoklee           #+#    #+#             */
/*   Updated: 2023/06/18 17:53:11 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_env_order(t_lst *ev)
{
	int	n;

	n = 1;
	while (ev != NULL)
	{
		ev->size = n++;
		ev = ev->nxt;
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
			}
			j = j->nxt;
		}
		i = i->nxt;
	}
}

int	print_export(t_lst *ev, int size)
{
	int		n;
	t_lst	*val;
	t_lst	*tmp;

	set_env_order(ev);
	sort_env(ev);
	n = 0;
	while (++n < size)
	{
		tmp = ev;
		while (tmp->size != n)
			tmp = tmp->nxt;
		printf("declare -x %s=", ((t_hash *)tmp->data)->key);
		val = ((t_hash *)tmp->data)->value;
		while (val != NULL)
		{
			printf("%c", *(char *)val->data);
			val = val->nxt;
		}
		printf("\n");
	}
	return (0);
}
