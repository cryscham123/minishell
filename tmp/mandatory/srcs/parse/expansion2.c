/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 23:15:43 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 00:47:21 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static t_lst	*ft_get_wild(t_lst *ret, int info)
{
	struct dirent	*item;
	DIR				*dp;
	t_lst			*to_push;

	dp = opendir("./");
	while (dp != NULL)
	{
		item = readdir(dp);
		if (item == NULL)
			break ;
		if (ft_strcmp(item->d_name, ".") == 0 \
			|| ft_strcmp(item->d_name, "..") == 0)
			continue ;
		to_push = mk_str_node(item->d_name, info);
		if (to_push == NULL)
		{
			closedir(dp);
			return (ft_lst_free(ret));
		}
		lst_push(&ret, to_push);
	}
	closedir(dp);
	if (ret == NULL)
		ret = mk_str_node("*", info);
	return (ret);
}

char	*ft_trans_ev_help2(char **s, char *target)
{
	char	*num;
	char	*ret;

	num = ft_itoa(g_status, 1);
	ret = ft_append(target, num, 0);
	free(num);
	(*s)++;
	return (ret);
}

t_lst	*ft_find_wild(t_lst *to_find, int *info, t_lst *ret)
{
	t_lst	*to_push;
	char	*tmp;

	while (to_find != NULL)
	{
		tmp = NULL;
		if (ft_strcmp(to_find->data, "*") == 0)
			to_push = ft_get_wild(NULL, *info);
		else
		{
			tmp = ft_delete_quote(to_find->data, info);
			if (tmp == NULL)
				return (ft_lst_free(ret));
			to_push = mk_lst(tmp, F_DATA_CHAR, *info);
		}
		if (to_push == NULL)
		{
			free(tmp);
			return (ft_lst_free(ret));
		}
		lst_push(&ret, to_push);
		to_find = to_find->nxt;
	}
	return (ret);
}

char	*ft_delete_quote(char *data, int *info)
{
	char	*ret;
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	ret = ft_calloc(ft_strlen(data) + 1);
	if (ret == NULL)
		return (NULL);
	while (*data)
	{
		if (*data == '\'' && flag != F_DQUOTE)
			flag ^= F_QUOTE;
		else if (*data == '\"' && flag != F_QUOTE)
			flag ^= F_DQUOTE;
		else
			ret[i++] = *data;
		if (*info == F_DEL && flag != 0)
			(*info) |= F_NO_TRANS;
		data++;
	}
	return (ret);
}
