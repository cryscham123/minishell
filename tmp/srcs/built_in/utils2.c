/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 02:03:58 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static char	**ft_av_env_help(char ***av, t_lst *ev)
{
	int		i;
	t_lst	*to_push;
	char	**env;

	while (**av != NULL)
	{
		i = ft_str_find(**av, '=');
		if (i == -1)
			break ;
		ft_unset_unit(**av, ev, 0);
		to_push = mk_lst(**av, F_DATA_CHAR, i);
		if (to_push == NULL)
			return (NULL);
		lst_push(&ev, to_push);
		(*av)++;
	}
	env = mk_argv(ev->nxt);
	return (env);
}

int	ft_av_env(char **av, t_lst *ev)
{
	pid_t	pid;
	char	**env;
	int		flag;

	ft_signal(SIG_IGN, SIG_IGN, 0);
	pid = fork();
	if (pid < 0)
		return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
	else if (pid == 0)
	{
		env = ft_av_env_help(&av, ev);
		if (env == NULL)
			exit(ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
		if (*av == NULL)
			flag = ft_env(NULL, env, NULL);
		else
			flag = ft_extern_cmd(av, env, ev, 1);
		ft_argv_free(env);
		exit(flag);
	}
	else
		waitpid(-1, &flag, 0);
	return (cal_flag(flag));
}

int	ft_av_exit(char *data)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	i = 0;
	sign = -(data[i] == '-') + (data[i] != '-');
	i += (data[i] == '+' || data[i] == '-');
	while (data[i])
	{
		if (!(data[i] >= '0' && data[i] <= '9') \
			|| (res == 214748364 && data[i] > ('7' + (sign == -1))))
		{
			ft_putstr_fd("exit\n", 1);
			exit(ft_error(F_ERROR_EXIT, F_EXIT_STATUS_EXIT));
		}
		res = res * 10 + data[i] - '0';
		i++;
	}
	if (((data[0] == '-' || data[0] == '+') && i == 1) || i == 0)
	{
		ft_putstr_fd("exit\n", 1);
		exit(ft_error(F_ERROR_EXIT, F_EXIT_STATUS_EXIT));
	}
	return (res * sign);
}
