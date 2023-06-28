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

int	echo_option_chk(t_lst *str)
{
	if (*(char *)str->data != '-' || str->nxt == NULL)
		return (0);
	str = str->nxt;
	while (str != NULL)
	{
		if (*(char *)str->data != 'n')
			return (0);
		str = str->nxt;
	}
	return (1);
}

int	ft_exit_code(t_lst *data)
{
	char		*tmp;
	long long	res;
	int			sign;
	int			i;

	tmp = ft_c_str(data, NULL, 0, -1);
	if (tmp == NULL)
		exit(ft_error(F_ERROR_MEM));
	res = 0;
	i = 0;
	sign = -(tmp[i] == '-') + (tmp[i] != '-');
	i += (tmp[i] == '+' || tmp[i] == '-');
	while (tmp[i])
	{
		if (!(tmp[i] >= '0' && tmp[i] <= '9') \
			|| (res == 214748364 && tmp[i] > ('7' + (sign == -1))))
			exit(ft_error(F_ERROR_EXIT));
		res = res * 10 + tmp[i] - '0';
		i++;
	}
	if (((tmp[0] == '-' || tmp[0] == '+') && i == 1) || i == 0)
		exit(ft_error(F_ERROR_EXIT));
	free(tmp);
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
