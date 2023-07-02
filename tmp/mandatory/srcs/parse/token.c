/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 03:23:18 by hyunghki         ###   ########.fr       */
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

static int	mk_token_node_help(t_token *target, t_lst *cmp)
{
	int		flag;
	t_lst	*to_push;

	if (cmp == NULL)
		return (1);
	flag = 0;
	while (cmp != NULL)
	{
		to_push = NULL;
		if (ft_split(cmp->data, "><", &flag, &to_push) != 0)
		{
			ft_lst_free(to_push);
			return (1);
		}
		ft_inject_token(target, to_push);
		cmp = cmp->nxt;
	}
	if (flag != 0)
		return (ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX));
	return (0);
}

static t_token	*mk_token_node(char *s, t_lst *ev)
{
	t_token	*target;
	t_lst	*cmp;

	if (*s == '\0')
	{
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
		return (NULL);
	}
	target = ft_calloc(sizeof(t_token));
	if (target == NULL)
		return (NULL);
	target->fd[1] = 1;
	cmp = ft_split_space(s);
	if (mk_token_node_help(target, cmp) != 0)
	{
		ft_node_free(target, F_DATA_TOKEN, 0);
		return (ft_lst_free(cmp));
	}
	ft_lst_free(cmp);
	if (ft_expansion(target, ev) != 0)
	{
		ft_node_free(target, F_DATA_TOKEN, 0);
		return (NULL);
	}
	return (target);
}

static t_lst	*mk_token_lst_help(t_lst *line, t_lst *ev)
{
	t_lst	*target;
	t_token	*tmp;
	t_lst	*to_push;

	target = NULL;
	while (line != NULL)
	{
		tmp = mk_token_node(ft_trim(line->data), ev);
		if (tmp == NULL)
			return (ft_lst_free(target));
		to_push = mk_lst(tmp, F_DATA_TOKEN, 0);
		if (to_push == NULL)
		{
			ft_node_free(tmp, F_DATA_TOKEN, 0);
			return (ft_lst_free(target));
		}
		lst_push(&target, to_push);
		line = line->nxt;
	}
	return (target);
}

t_lst	*mk_token_lst(char *s, t_lst *ev)
{
	int		flag;
	t_lst	*line;
	t_lst	*target;

	if (*s == '\0')
	{
		g_status = 0;
		return (NULL);
	}
	flag = F_PIPE;
	line = NULL;
	if (ft_split(s, "|", &flag, &line) != 0)
		return (ft_lst_free(line));
	if (flag != 0)
	{
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
		return (ft_lst_free(line));
	}
	target = mk_token_lst_help(line, ev);
	ft_lst_free(line);
	return (target);
}
