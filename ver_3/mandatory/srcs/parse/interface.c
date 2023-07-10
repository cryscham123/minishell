/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 02:16:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:29:07 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static t_lst	*mk_command(char *line, t_lst *ev)
{
	t_lst	*target;
	t_lst	*ret;

	if (*line == '\0')
	{
		g_status = 0;
		return (NULL);
	}
	target = ft_split_space(line);
	if (target == NULL)
		return (NULL);
	ret = mk_token_lst(target, ev);
	ft_lst_free(target);
	return (ret);
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
		tv = mk_command(ft_trim(line), ev);
		if (tv != NULL)
		{
			g_status = ft_exe(tv, ev);
			ft_lst_free(tv);
		}
	}
	free(line);
}
