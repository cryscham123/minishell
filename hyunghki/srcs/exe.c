/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 13:11:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/18 14:24:09 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
static char	**mk_av(t_lst *data)
{
	char	**target;
	int		i;
	int		j;

	target = ft_calloc(data->size);
	if (target == NULL)
		return (ft_error(f_error_mem));
	i = 0;
	while (data != NULL)
	{
		target[i++] = ft_c_str(data->data, -1, 1);
		if (target[i - 1] == NULL)
		{
			j = 0;
			while (j < i - 1)
				free(target[j++]);
			free(target);
			return (ft_error(f_error_mem));
		}
		data = data->nxt;
	}
	return (target);
}
static int	ft_extern(t_lst *data, t_lst *ev, char **env, int is_single)
{
	pid_t	pid;
	char	*path;
	char	**argv;
	int		flag;

	path = mk_path;
	argv = mk_av(data);
	if (is_single)
	{
		pid = fork();
		if (pid == 0 && execve(path, argv, env) < 0)
			return (ft_error(f_error_exe));
	}
	else if (execve(path, argv, ev) < 0)
		return (ft_error(f_error_exe));
	return (flag);
}
**/

static int	ft_exe_cmd(t_token *data, t_lst *ev, char **env, int is_single)
{
	int	flag;
	int	in;
	int	out;

	(void)env;
	(void)is_single;
	if (ft_redirection(data, data->redirection, ev) != 0)
		return (1);
	in = dup(0);
	out = dup(1);
	dup2(data->fd[0], 0);
	dup2(data->fd[1], 1);
	flag = 0;
	if (data->argv != NULL)
	   flag = ft_built_in_cmd(data->argv, ev);
	/**
	if (flag == 2)
		flag = ft_extern(data->argv, env, is_single);
		**/
	dup2(in, 0);
	dup2(out, 1);
	close(in);
	close(out);
	return (flag);
}

int	ft_exe(t_lst *tv, t_lst *ev, char **env)
{
	int		is_single;
	int		flag;

	is_single = (tv->nxt == NULL);
	while (tv != NULL)
	{
		flag = ft_exe_cmd(tv->data, ev, env, is_single);
		tv = tv->nxt;
	}
	return (flag);
}
