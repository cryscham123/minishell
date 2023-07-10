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

static void	ft_inject_token(t_token *target, t_lst *to_push)
{
	while (to_push != NULL)
	{
		if (to_push->info == 0)
			lst_push(&target->argv, to_push);
		else
			lst_push(&target->redir, to_push);
		to_push = to_push->nxt;
		if (to_push != NULL && to_push->prev != NULL)
		{
			to_push->prev->nxt = NULL;
			to_push->prev = NULL;
		}
	}
}

static t_token	*mk_token_node(t_lst **piped, int *flag, t_lst *ev)
{
	t_token	*ret;
	t_lst	*tmp;
	t_lst	*to_push;

	ret = ft_calloc(sizeof(t_token));
	if (ret == NULL)
		return (NULL);
	ret->fd[1] = 1;
	tmp = *piped;
	tmp->info = 0;
	while (tmp != NULL && tmp->info != F_PIPE)
	{
		to_push = NULL;
		if (ft_split(tmp->data, "><", flag, &to_push) != 0)
			return (ft_node_free(ret, F_DATA_TOKEN, 0));
		ft_inject_token(ret, to_push);
		tmp = tmp->nxt;
	}
	if (*flag != 0)
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
	if (*flag != 0 || ft_expansion(ret, ev) != 0)
		return (ft_node_free(ret, F_DATA_TOKEN, 0));
	*piped = tmp;
	return (ret);
}

static t_lst	*mk_token_lst_help(t_lst *piped, t_lst *ev, int flag)
{
	t_lst	*ret;
	t_lst	*to_push;
	t_token	*token;

	ret = NULL;
	while (piped != NULL)
	{
		token = mk_token_node(&piped, &flag, ev);
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
	if (flag != 0)
	{
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
		return (ft_lst_free(ret));
	}
	return (ret);
}

t_lst	*mk_token_lst(t_lst *target, t_lst *ev)
{
	int		flag;
	t_lst	*ret;
	t_lst	*piped;
	t_lst	*to_push;

	piped = NULL;
	flag = F_PIPE;
	while (target != NULL)
	{
		to_push = NULL;
		if (ft_split(target->data, "|", &flag, &to_push) != 0)
			return (ft_lst_free(piped));
		lst_push(&piped, to_push);
		target = target->nxt;
	}
	if (flag != 0)
	{
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
		return (ft_lst_free(piped));
	}
	ret = mk_token_lst_help(piped, ev, 0);
	ft_lst_free(piped);
	return (ret);
}
