/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/18 12:50:17 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_lst *argv, t_lst *ev)
{
	char	*tmp;
	t_lst	*pwd;

	tmp = ft_c_str(argv->data, -1, 1);
	if (tmp == NULL)
		return (ft_error(f_error_mem));
	if (chdir(tmp) != 0)
	{
		free(tmp);
		return (ft_error(f_error_file));
	}
	free(tmp);
	if (ft_pwd(&pwd) != 0)
		return (1);
	while (ev != NULL)
	{
		if (ft_strcmp(((t_hash *)ev->data)->key, "PWD") == 0)
		{
			ft_node_free(((t_hash *)ev->data)->value, f_data_char);
			((t_hash *)ev->data)->value = pwd;
			break ;
		}
		ev = ev->nxt;
	}
	return (0);
}

int	ft_pwd(t_lst **buf)
{
	char	*tmp;
	t_lst	*ret;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
		return (ft_error(f_error_mem));
	if (buf != NULL)
	{
		ret = mk_str_lst(tmp);
		free(tmp);
		if (ret == NULL)
			return (ft_error(f_error_mem));
		(*buf) = ret;
		return (0);
	}
	printf("%s\n", tmp);
	free(tmp);
	return (0);
}

int	ft_exit(t_lst *argv)
{
	char	*tmp;
	int		i;
	int		num;

	if (argv->nxt->nxt != NULL)
		return (ft_error(f_error_arg));
	tmp = ft_c_str(((t_lst *)argv->nxt)->data, -1, 1);
	if (tmp == NULL)
		return (ft_error(f_error_mem));
	num = 0;
	i = 0;
	while (tmp[i])
	{
		if (!(tmp[i] >= '0' && tmp[i] <= '9'))
		{
			free(tmp);
			return (ft_error(f_error_exit));
		}
		num = num * 10 + tmp[i++] - '0';
	}
	free(tmp);
	printf("exit %d\n", num);
	exit(num);
}

int	ft_built_in_cmd(t_lst *argv, t_lst *ev)
{
	char	*cmd;

	cmd = ft_c_str(argv->data, -1, 1);
	if (cmd == NULL)
		return (ft_error(f_error_mem));
	if (ft_strcmp(cmd, "echo") == 0)
	   return (ft_echo(argv->nxt, NULL, 1));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(argv->nxt, ev));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd(NULL));
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(argv->nxt, ev, 0));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(argv->nxt, ev, 0));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env((ev->nxt)));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit((argv)));
	return (2);
}
