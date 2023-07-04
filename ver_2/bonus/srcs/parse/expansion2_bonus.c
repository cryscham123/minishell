/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:39:25 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 13:27:39 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

extern int	g_status;

static int	wild_pattern_chk_help(char *pattern, char *s, int flag)
{
	if (*pattern == '\0' && *s == '\0')
		return (0);
	if (*pattern == '\'' && flag != F_DQUOTE)
		return (wild_pattern_chk_help(pattern + 1, s, flag ^ F_QUOTE));
	if (*pattern == '\"' && flag != F_QUOTE)
		return (wild_pattern_chk_help(pattern + 1, s, flag ^ F_DQUOTE));
	if (flag == 0 && *pattern == '*' && *(pattern + 1) != '\0' && *s == '\0')
		return (wild_pattern_chk_help(pattern + 1, s, flag));
	if (*pattern == *s && *s != '*')
		return (wild_pattern_chk_help(pattern + 1, s + 1, flag));
	if (flag == 0 && *pattern == '*')
	{
		if (wild_pattern_chk_help(pattern + 1, s, flag) == 0)
			return (0);
		if (wild_pattern_chk_help(pattern, s + 1, flag) == 0)
			return (0);
	}
	return (1);
}

static int	wild_pattern_chk(char *pattern, char *s)
{
	if (pattern == NULL || ft_str_find(pattern, '*') == -1)
		return (1);
	if (*s == '.' && *pattern != '.')
		return (1);
	return (wild_pattern_chk_help(pattern, s, 0));
}

static t_lst	*ft_get_wild(char *data, int info, t_lst *ret)
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
		to_push = mk_str_node(item->d_name, info, 0);
		if (to_push == NULL)
		{
			closedir(dp);
			return (ft_lst_free(ret));
		}
		lst_push(&ret, to_push);
	}
	closedir(dp);
	if (ret == NULL)
		ret = mk_str_node(data, info, 1);
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

	while (to_find != NULL)
	{
		to_push = ft_get_wild(to_find->data, *info, NULL);
		if (to_push == NULL)
			return (ft_lst_free(ret));
		lst_push(&ret, to_push);
		to_find = to_find->nxt;
	}
	return (ret);
}
