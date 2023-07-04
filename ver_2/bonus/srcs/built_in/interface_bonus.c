/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 12:11:22 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in_bonus.h"

int	ft_echo(char **av)
{
	int	flag;

	flag = 0;
	while (*av != NULL && echo_option_chk(*av) == 1)
	{
		flag = 1;
		av++;
	}
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

	if (*av == NULL || **av == '\0')
		return (0);
	flag = ft_av_validate_dir(av);
	if (flag != 0)
		return (1);
	flag = ft_set_ev_pwd("OLDPWD", ev);
	chdir(*av);
	if (flag == 0)
		flag = ft_set_ev_pwd("PWD", ev);
	return (flag);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_error(F_ERROR_ACCESS, F_EXIT_STATUS_FILE));
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
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
		flag = ft_av_validate_ev(*av);
		if (flag == 0)
		{
			to_push = mk_str_node(*av, ft_str_find(*av, '='), 0);
			if (to_push == NULL)
				return (1);
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
		flag = ft_av_validate_ev(*av);
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
