/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:32:22 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in_bonus.h"

extern int	g_status;

int	ft_env(char **av, char **env, t_lst *ev)
{
	if (av != NULL && *av != NULL)
		return (ft_av_env(av, ev));
	while (*env != NULL)
	{
		ft_putstr_fd(*env, 1);
		ft_putstr_fd("\n", 1);
		env++;
	}
	return (0);
}

int	ft_exit(char **av)
{
	int		num;

	num = g_status;
	if (*av != NULL)
	{
		num = ft_av_exit(*av);
		if (*(av + 1) != NULL)
			return (ft_error(F_ERROR_ARG, F_EXIT_STATUS_BUILTIN));
	}
	ft_putstr_fd("exit\n", 1);
	exit(num);
}
