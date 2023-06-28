/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:28:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	ft_cd(t_lst *argv, t_lst *ev)
{
	char	*tmp;

	if (argv == NULL)
		return (0);
	if (ft_set_ev_pwd("OLDPWD", ev, NULL) != 0)
		return (1);
	tmp = ft_c_str(argv->data, NULL, 0, -1);
	if (tmp == NULL)
		return (ft_error(F_ERROR_MEM));
	if (chdir(tmp) != 0)
	{
		free(tmp);
		return (ft_error(F_ERROR_FILE));
	}
	free(tmp);
	if (ft_set_ev_pwd("PWD", ev, NULL) != 0)
		return (1);
	return (0);
}

int	ft_pwd(void)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
		return (ft_error(F_ERROR_FILE));
	printf("%s\n", tmp);
	free(tmp);
	return (0);
}

int	ft_exit(t_lst *argv)
{
	int		num;

	num = g_status;
	if (argv != NULL && argv->nxt != NULL)
	{
		num = ft_exit_code(argv->nxt->data);
		if (argv->nxt->nxt != NULL)
			return (ft_error(F_ERROR_ARG));
	}
	printf("exit\n");
	exit(num);
}

int	ft_echo(t_lst *argv)
{
	int		option_flag;
	char	*tmp;

	option_flag = 0;
	if (argv != NULL && echo_option_chk(argv->data) != 0)
	{
		argv = argv->nxt;
		option_flag = 1;
	}
	while (argv != NULL)
	{
		tmp = ft_c_str(argv->data, NULL, 0, -1);
		if (tmp == NULL)
			return (ft_error(F_ERROR_MEM));
		printf("%s", tmp);
		free(tmp);
		if (argv->nxt != NULL)
			printf(" ");
		argv = argv->nxt;
	}
	if (option_flag == 0)
		printf("\n");
	return (0);
}

int	ft_built_in_cmd(t_lst *argv, t_lst *ev)
{
	char	*cmd;
	int		flag;

	flag = F_STATUS_NO_BUILTIN;
	cmd = ft_c_str(argv->data, NULL, 0, -1);
	if (cmd == NULL)
		flag = ft_error(F_ERROR_MEM);
	else if (ft_strcmp(cmd, "echo") == 0)
		flag = ft_echo(argv->nxt);
	else if (ft_strcmp(cmd, "cd") == 0)
		flag = ft_cd(argv->nxt, ev);
	else if (ft_strcmp(cmd, "pwd") == 0)
		flag = ft_pwd();
	else if (ft_strcmp(cmd, "export") == 0)
		flag = ft_export(argv->nxt, ev, 0);
	else if (ft_strcmp(cmd, "unset") == 0)
		flag = ft_unset(argv->nxt, ev, 0);
	else if (ft_strcmp(cmd, "env") == 0)
		flag = ft_env(ev->nxt);
	else if (ft_strcmp(cmd, "exit") == 0)
		flag = ft_exit(argv);
	free(cmd);
	return (flag);
}
