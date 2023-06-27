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
	if (tmp == NULL)
	{
		ft_error(F_ERROR_FILE);
		return (0);
	}
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
