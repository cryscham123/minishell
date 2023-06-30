/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 02:16:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 04:26:30 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_test(t_lst *tv)
{
	while (tv != NULL)
	{
		ft_putstr_fd("================ test ===============\n\n", 1);
		t_token *target = tv->data;
		t_lst *argv = target->argv;
		ft_putstr_fd("ARGV:\n", 1);
		while (argv != NULL)
		{
			ft_putstr_fd(argv->data, 1);
			ft_putstr_fd("\n", 1);
			argv = argv->nxt;
		}
		t_lst *redir = target->redir;
		ft_putstr_fd("redir:\n", 1);
		while (redir != NULL)
		{
			if (redir->info == F_INPUT)
				ft_putstr_fd("input: ", 1);
			if (redir->info == F_DEL)
				ft_putstr_fd("del: ", 1);
			if (redir->info == F_OUTPUT)
				ft_putstr_fd("output: ", 1);
			if (redir->info == F_APPEND)
				ft_putstr_fd("append: ", 1);
			ft_putstr_fd(redir->data, 1);
			ft_putstr_fd("\n", 1);
			argv = argv->nxt;
		}
		tv = tv->nxt;
	}
}

void	ft_parse(t_lst *ev)
{
	char	*line;
	t_lst	*tv;

	ft_signal(sigint_handler, SIG_IGN, 0);
	line = readline("\e[95mminishell$\e[0m \e[s");
	if (line == NULL)
	{
		ft_putstr_fd("\e[u\e[B\e[Aexit\n", 1);
		exit(0);
	}
	if (*line)
	{
		add_history(line);
		tv = mk_token_lst(ft_trim(line), ev);
		if (tv != NULL)
		{
			//g_status = ft_exe(tv, ev);
			ft_test(tv);
			ft_lst_free(tv);
		}
	}
	free(line);
}
