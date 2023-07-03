/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 02:16:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:34:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

extern int	g_status;

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
			g_status = ft_exe(tv, ev);
			ft_lst_free(tv);
		}
	}
	free(line);
}
