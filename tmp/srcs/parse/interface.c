/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:22:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	mk_token_node_help(t_token *target, t_lst *tmp)
{
	t_lst	*to_push;
	int		i;
	int		flag;

	if (tmp == NULL)
		return (1);
	while (tmp != NULL)
	{
		i = ft_chk_flag(&s, &flag);
		if (flag < 0)
			return (ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX));
		to_push = ft_mk_str(s, i - 1);
		if (to_push == NULL)
			return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
		to_push = mk_lst(to_push, F_DATA_STRING, flag);
		if (flag = 0)
			lst_push(&target->argv, to_push);
		else
			lst_push(&target->redir, to_push);
		tmp = tmp->nxt;
	}
	return (0);
}

static t_token	*mk_token_node(char *s, t_lst *ev)
{
	t_token	*target;
	t_lst	*tmp;

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
	tmp = ft_split_space(s);
	if (mk_token_node_help(target, tmp) != 0)
	{
		ft_lst_free(tmp);
		return (ft_lst_free(target));
	}
	ft_lst_free(tmp);
	if (ft_expansion(target->argv) != 0 || ft_expansion(target->redir) != 0)
		return (ft_lst_free(target));
	return (target);
}

t_lst	*mk_token_lst(char *s, t_lst *ev)
{
	t_lst	*tmp;
	t_token	*to_push;
	t_lst	*target;

	if (*s == '|')
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
	if (*s == '\0' || *s == '|')
		return (NULL);
	tmp = ft_split(s, "|");
	if (tmp == NULL)
		return (NULL);
	while (tmp != NULL)
	{
		to_push = mk_token_node(ft_trim(tmp->data), ev);
		if (to_push == NULL)
		{
			ft_lst_free(tmp);
			return (ft_lst_free(target));
		}
		lst_push(&target, to_push);
		tmp = tmp->nxt;
	}
	ft_lst_free(tmp);
	return (target);
}

void	ft_parse(t_lst *ev)
{
	char	*line;
	t_lst	*tv;

	ft_signal(sigint_handler, SIG_IGN, 0);
	line = readline("\e[95mminishell$\e[0m [s");
	if (line == NULL)
	{
		printf("\e[u\e[B\e[Aexit\n");
		exit(0);
	}
	if (*line)
	{
		add_history(line);
		tv = mk_token_lst(ft_trim(line), ev);
		if (tv != NULL)
		{
			g_status = ft_exe(tv, ev);
			lst_free(tv);
		}
	}
	free(line);
}
