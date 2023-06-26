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

int	ft_cd(t_lst *argv, t_lst *ev, char *tmp)
{
	t_hash	*pwd;

	if (argv == NULL)
		return (0);
	tmp = ft_c_str(argv->data, NULL, 0, -1);
	if (tmp == NULL)
		return (ft_error(F_ERROR_MEM));
	if (chdir(tmp) != 0)
	{
		free(tmp);
		return (ft_error(F_ERROR_FILE));
	}
	free(tmp);
	pwd = ft_calloc(sizeof(t_hash));
	if (pwd == NULL || ft_pwd(pwd) != 0)
	{
		ft_node_free(pwd, F_DATA_HASH);
		return (1);
	}
	ft_chk_key("PWD", ev, 1);
	lst_push(&ev, mk_lst(pwd, 0));
	return (0);
}

int	ft_pwd(t_hash *buf)
{
	char	*tmp;
	t_lst	*ret;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
		return (ft_error(F_ERROR_FILE));
	if (buf != NULL)
	{
		ret = mk_str_lst(tmp);
		free(tmp);
		if (ret == NULL)
			return (ft_error(F_ERROR_MEM));
		buf->value = ret;
		buf->key = ft_substr("PWD", 3);
		if (buf->key == NULL)
		{
			ft_lst_free(buf->value, NULL, F_DATA_CHAR, F_ERROR_MEM);
			return (1);
		}
		return (0);
	}
	printf("%s\n", tmp);
	free(tmp);
	return (0);
}

int	ft_exit(t_lst *argv, int i)
{
	char	*tmp;
	int		num;

	num = 0;
	if (argv != NULL && argv->nxt != NULL)
	{
		if (argv->nxt->nxt != NULL)
			return (ft_error(F_ERROR_ARG));
		tmp = ft_c_str(argv->nxt->data, NULL, 0, -1);
		if (tmp == NULL)
			return (ft_error(F_ERROR_MEM));
		while (tmp[i])
		{
			if (!(tmp[i] >= '0' && tmp[i] <= '9'))
			{
				free(tmp);
				return (ft_error(F_ERROR_EXIT));
			}
			num = num * 10 + tmp[i++] - '0';
		}
		free(tmp);
	}
	printf("exit\n");
	exit(num);
}

int	ft_echo(t_lst *argv, char *tmp, int flag)
{
	if (argv != NULL)
	{
		tmp = ft_c_str(argv->data, NULL, 0, -1);
		if (tmp == NULL)
			return (ft_error(F_ERROR_MEM));
		flag = ft_strcmp(tmp, "-n");
		free(tmp);
	}
	if (flag == 0)
		argv = argv->nxt;
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
	if (flag)
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
		flag = ft_echo(argv->nxt, NULL, 1);
	else if (ft_strcmp(cmd, "cd") == 0)
		flag = ft_cd(argv->nxt, ev, NULL);
	else if (ft_strcmp(cmd, "pwd") == 0)
		flag = ft_pwd(NULL);
	else if (ft_strcmp(cmd, "export") == 0)
		flag = ft_export(argv->nxt, ev, 0);
	else if (ft_strcmp(cmd, "unset") == 0)
		flag = ft_unset(argv->nxt, ev, 0);
	else if (ft_strcmp(cmd, "env") == 0)
		flag = ft_env(ev->nxt);
	else if (ft_strcmp(cmd, "exit") == 0)
		flag = ft_exit(argv, 0);
	free(cmd);
	return (flag);
}
