/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 05:40:45 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (1);
		while (to_push != NULL)
		{
			if (to_push->info == 0)
				lst_push(&target->argv, to_push);
			else
				lst_push(&target->redir, to_push);
			to_push = to_push->nxt;
		}
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
	{
		ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM);
		return (NULL);
	}
	target->fd[1] = 1;
	cmp = ft_split_space(s);
	if (mk_token_node_help(target, cmp) != 0)
	{
		ft_node_free(target, F_DATA_TOKEN, 0);
		return (ft_lst_free(cmp));
	}
	ft_lst_free(cmp);
	(void)ev;
	/**
	if (ft_expansion(target->argv) != 0 || ft_expansion(target->redir) != 0)
		return (ft_lst_free(target));
	**/
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
			ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM);
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
		return (NULL);
	if (flag != 0)
	{
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
		return (ft_lst_free(line));
	}
	target = mk_token_lst_help(line, ev);
	ft_lst_free(line);
	return (target);
}
