/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 05:33:26 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

extern int	g_status;

int	ft_env(t_lst *ev)
{
	ev = ev->nxt;
	while (ev != NULL)
	{
		ft_putstr_fd(ev->data, 1);
		ft_putstr_fd("\n", 1);
		ev = ev->nxt;
	}
	return (0);
}

int	ft_exit(char **av, int is_forked)
{
	int		num;

	num = g_status;
	if (*av != NULL)
	{
		num = ft_av_exit(*av, is_forked);
		if (*(av + 1) != NULL)
			return (ft_error(F_ERROR_ARG, F_EXIT_STATUS_BUILTIN));
	}
	if (!is_forked)
		ft_putstr_fd("exit\n", 1);
	exit(num);
}
