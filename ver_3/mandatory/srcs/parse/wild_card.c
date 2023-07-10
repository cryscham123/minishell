/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 02:13:14 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lst	*mk_c_str_lst(t_lst *data, int info)
{
	char	*tmp;
	t_lst	*ret;

	tmp = ft_c_str(data);
	if (tmp == NULL)
		return (NULL);
	ret = mk_lst(tmp, F_DATA_CHAR, info);
	if (ret == NULL)
	{
		free(tmp);
		return (NULL);
	}
	return (ret);
}

static int	wild_pattern_chk_help(t_lst *pattern, char *s, int flag)
{
	char	c;

	if (pattern == NULL)
		c = '\0';
	else
		c = *(char *)pattern->data;
	if (c == '\0' && *s == '\0')
		return (0);
	if (c == '\'' && pattern->info == F_NO_PARSE && flag != F_DQUOTE)
		return (wild_pattern_chk_help(pattern->nxt, s, flag ^ F_QUOTE));
	if (c == '\"' && pattern->info == F_NO_PARSE && flag != F_QUOTE)
		return (wild_pattern_chk_help(pattern->nxt, s, flag ^ F_DQUOTE));
	if (flag == 0 && c == '*' && pattern->nxt != NULL && *s == '\0')
		return (wild_pattern_chk_help(pattern->nxt, s, flag));
	if (flag == 0 && c == '*')
	{
		if (wild_pattern_chk_help(pattern->nxt, s, flag) == 0)
			return (0);
		if (wild_pattern_chk_help(pattern, s + 1, flag) == 0)
			return (0);
	}
	if (c == *s)
		return (wild_pattern_chk_help(pattern->nxt, s + 1, flag));
	return (1);
}

static int	wild_pattern_chk(t_lst *pattern, char *s)
{
	if (pattern == NULL || ft_lst_find(pattern, '*') == -1)
		return (1);
	if (*s == '.' && *(char *)pattern->data != '.')
		return (1);
	return (wild_pattern_chk_help(pattern, s, 0));
}

static t_lst	*ft_get_wild(t_lst *data, int info, t_lst *ret)
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
		if (wild_pattern_chk(data, item->d_name) != 0)
			continue ;
		to_push = mk_str_node(item->d_name, -1, info);
		if (to_push == NULL)
		{
			closedir(dp);
			return (ft_lst_free(ret));
		}
		lst_push(&ret, to_push);
	}
	closedir(dp);
	if (ret == NULL)
		ret = mk_c_str_lst(data, info);
	return (ret);
}

t_lst	*ft_find_wild(t_lst *to_find, int info, t_lst *ret)
{
	t_lst	*to_push;

	while (to_find != NULL)
	{
		to_push = ft_get_wild(to_find->data, info, NULL);
		if (to_push == NULL)
			return (ft_lst_free(ret));
		lst_push(&ret, to_push);
		to_find = to_find->nxt;
	}
	return (ret);
}
