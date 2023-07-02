/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 02:04:44 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe.h"

static int	ft_redirection(t_token *token, t_lst *redir)
{
	while (redir != NULL)
	{
		if (ft_open(token) != 0)
			return (1);
		redir = redir->nxt;
	}
	return (0);
}

int	ft_built_in_cmd(char **argv, char **env, t_lst *ev)
{
	int		flag;

	flag = F_STATUS_NO_BUILTIN;
	if (ft_strcmp(argv[0], "echo") == 0)
		flag = ft_echo(argv + 1);
	else if (ft_strcmp(argv[0], "cd") == 0)
		flag = ft_cd(argv + 1, ev);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		flag = ft_pwd();
	else if (ft_strcmp(argv[0], "export") == 0)
		flag = ft_export(argv + 1, ev);
	else if (ft_strcmp(argv[0], "unset") == 0)
		flag = ft_unset(argv + 1, ev);
	else if (ft_strcmp(argv[0], "env") == 0)
		flag = ft_env(argv + 1, env, ev);
	else if (ft_strcmp(argv[0], "exit") == 0)
		flag = ft_exit(argv + 1);
	return (flag);
}

int	ft_extern_cmd(char **av, char **env, t_lst *ev, int is_forked)
{
	pid_t	pid;
	t_lst	*path;
	int		flag;

	flag = 0;
	path = ft_env_find(ev, "PATH");
	if (!is_forked)
	{
		ft_signal(SIG_IGN, SIG_IGN, 0);
		pid = fork();
		if (pid < 0)
			return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
		else if (pid == 0)
			ft_exe_extern(path->data + (path->info + 1), av, env);
		else
			waitpid(-1, &flag, 0);
	}
	else
		ft_exe_extern(path->data + (path->info + 1), av, env);
	return (flag);
}

static int	ft_exe_help(t_token *data, t_lst *ev, t_lst *tv, int is_forked)
{
	char	**av;
	char	**env;
	int		flag;
	int		fd_tmp[2];

	flag = ft_redirection(data, data->redir);
	if (flag == 0 && data->argv != NULL)
	{
		av = mk_argv(data->argv);
		env = mk_argv(ev->nxt);
		if (av == NULL || env == NULL)
			flag = F_EXIT_STATUS_MEM;
		fd_tmp[0] = dup(0);
		fd_tmp[1] = dup(1);
		ft_dup2(data->fd, tv);
		flag = ft_built_in_cmd(av, env, ev);
		if (flag == F_STATUS_NO_BUILTIN)
			flag = ft_extern_cmd(av, env, ev, is_forked);
		ft_argv_free(av);
		ft_argv_free(env);
		ft_dup2(fd_tmp, NULL);
	}
	if (is_forked)
		exit(flag);
	return (cal_flag(flag));
}

int	ft_exe(t_lst *tv, t_lst *ev)
{
	pid_t	pid;
	int		n;
	int		flag;

	n = ft_lst_size(tv);
	if (n == 1)
		return (ft_exe_help(tv->data, ev, tv, 0));
	if (ft_pipe(tv) != 0)
		return (1);
	while (tv != NULL)
	{
		ft_signal(SIG_IGN, SIG_IGN, 0);
		pid = fork();
		if (pid < 0)
			return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
		else if (pid == 0)
			ft_exe_help(tv->data, ev, tv, 1);
		ft_close(((t_token *)tv->data)->fd, NULL);
		tv = tv->nxt;
	}
	while (n--)
		waitpid(-1, &flag, 0);
	return (cal_flag(flag));
}
