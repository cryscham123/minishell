/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:29:16 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static t_token	*mk_token_node(t_lst **splitted, t_lst *ev, t_token *ret)
{
	t_lst	*tmp;

	ret = ft_calloc(sizeof(t_token));
	if (ret == NULL)
		return (NULL);
	ret->fd[1] = 1;
	tmp = *splitted;
	tmp->info &= ~F_PIPE;
	while (tmp != NULL && (tmp->info & F_PIPE) == 0)
	{
		if (tmp->info == 0)
			lst_push(&ret->argv, tmp);
		else
			lst_push(&ret->redir, tmp);
		tmp = tmp->nxt;
		if (tmp != NULL && tmp->prev != NULL)
		{
			tmp->prev->nxt = NULL;
			tmp->prev = NULL;
		}
	}
	if (ft_expansion(ret, ev) != 0)
		return (ft_node_free(ret, F_DATA_TOKEN, 0));
	*splitted = tmp;
	return (ret);
}

static t_lst	*mk_token_lst_help(t_lst *splitted, t_lst *ev)
{
	t_lst	*ret;
	t_lst	*to_push;
	t_token	*token;

	ret = NULL;
	while (splitted != NULL)
	{
		token = mk_token_node(&splitted, ev, NULL);
		if (token == NULL)
			return (ft_lst_free(ret));
		to_push = mk_lst(token, F_DATA_TOKEN, 0);
		if (to_push == NULL)
		{
			ft_node_free(token, F_DATA_TOKEN, 0);
			return (ft_lst_free(ret));
		}
		lst_push(&ret, to_push);
	}
	return (ret);
}

t_lst	*mk_token_lst(t_lst *target, t_lst *ev)
{
	int		flag;
	t_lst	*ret;
	t_lst	*splitted;
	t_lst	*to_push;

	splitted = NULL;
	flag = F_PIPE;
	while (target != NULL)
	{
		to_push = NULL;
		if (ft_split(target->data, "|><", &flag, &to_push) != 0)
			return (ft_lst_free(splitted));
		lst_push(&splitted, to_push);
		target = target->nxt;
	}
	if (flag != 0)
	{
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
		return (ft_lst_free(splitted));
	}
	ret = mk_token_lst_help(splitted, ev);
	return (ret);
}
