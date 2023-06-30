/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 03:09:21 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	ft_echo(char **av)
{
	int	flag;

	flag = echo_option_chk(*av);
	if (flag == 1)
		av++;
	while (*av != NULL)
	{
		ft_putstr_fd(*av, 1);
		av++;
		if (*av != NULL)
			ft_putstr_fd(" ", 1);
	}
	if (flag != 1)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	ft_cd(char **av, t_lst *ev)
{
	int		flag;

	if (*av == NULL)
		return (0);
	flag = ft_av_validate_dir(av);
	if (flag != 0)
		return (1);
	if (ft_set_ev_pwd("OLDPWD", ev) != 0)
		return (1);
	chdir(*av);
	if (ft_set_ev_pwd("PWD", ev) != 0)
		return (1);
	return (0);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_error(F_ERROR_FILE, F_EXIT_STATUS_FILE));
	ft_putstr_fd(pwd, 1);
	free(pwd);
	return (0);
}

int	ft_export(char **av, t_lst *ev)
{
	int		flag;
	t_lst	*to_push;

	flag = 0;
	while (*av != NULL)
	{
		flag = ft_av_validate_ev(*av, ev);
		if (flag == 0)
		{
			to_push = mk_lst(*av, F_DATA_CHAR, ft_str_find(*av, '='));
			if (to_push == NULL)
				return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
			ft_unset_unit(*av, ev, flag);
			lst_push(&ev, to_push);
		}
		else if (flag == F_EV_NO_VALUE)
			flag = 0;
		else if (flag == F_EV_KEY_ERROR)
			flag = F_EXIT_STATUS_BUILTIN;
		av++;
	}
	return (flag);
}

int	ft_unset(char **av, t_lst *ev)
{
	int	flag;

	flag = 0;
	while (*av != NULL)
	{
		flag = ft_av_validate_ev(*av, ev);
		if (flag == F_EV_NO_VALUE)
		{
			ft_unset_unit(*av, ev, flag);
			flag = 0;
		}
		else
			flag = F_EXIT_STATUS_BUILTIN;
		av++;
	}
	return (flag);
}
