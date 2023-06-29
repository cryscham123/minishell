/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:28:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_set_ev_pwd(char *key, t_lst *ev, t_hash *pwd)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL && ft_strcmp("PWD", key) == 0)
		ft_error(F_ERROR_FILE);
	if (tmp == NULL)
		return (0);
	pwd = ft_calloc(sizeof(t_hash));
	if (pwd == NULL)
	{
		free(tmp);
		return (ft_error(F_ERROR_MEM));
	}
	pwd->key = ft_substr(key, -1);
	pwd->value = mk_str_lst(tmp);
	free(tmp);
	if (pwd->key == NULL || pwd->value == NULL)
	{
		ft_node_free(pwd, F_DATA_HASH);
		return (ft_error(F_ERROR_MEM));
	}
	ft_chk_key(key, ev, 1);
	lst_push(&ev, mk_lst(pwd, 0));
	return (0);
}

int	echo_option_chk(char *target)
{
	int	i;

	i = 1;
	if (target[0] != '-' || target[1] == '\0')
	{
		printf("%s", target);
		return (1);
	}
	while (target[i] == 'n')
		i++;
	if (target[i] == '\0' || (target[i] == ' ' && target[i + 1] == '\0'))
		return (2);
	printf("%s", target);
	return (1);
}

int	ft_exit_code(char *data)
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
			printf("exit\n");
			exit(ft_error(F_ERROR_EXIT));
		}
		res = res * 10 + data[i] - '0';
		i++;
	}
	if (((data[0] == '-' || data[0] == '+') && i == 1) || i == 0)
	{
		printf("exit\n");
		exit(ft_error(F_ERROR_EXIT));
	}
	return (res * sign);
}

int	chk_cd_argv(char *path)
{
	struct stat	buf;

	if (*path == '\0')
		return (-1);
	stat(path, &buf);
	if (access(path, F_OK) != 0)
		return (ft_error(F_ERROR_FILE));
	if (access(path, X_OK) != 0)
		return (ft_error(F_ERROR_FILE));
	if (!S_ISDIR(buf.st_mode))
		return (ft_error(F_ERROR_NOT_DIR));
	return (0);
}
