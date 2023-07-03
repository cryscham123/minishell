/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:33:50 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static char	*ft_trans_ev_help(char **s, t_lst *ev)
{
	t_lst	*to_find;
	char	*target;
	char	tmp;
	int		i;

	if (*(*s + 1) == '\'' || *(*s + 1) == '\"')
		return (NULL);
	(*s)++;
	i = 0;
	while ((*s)[i] && ft_str_find("| \t><$\'\"=*", (*s)[i]) == -1)
		i++;
	tmp = (*s)[i];
	(*s)[i] = '\0';
	to_find = ft_env_find(ev, *s);
	(*s)[i] = tmp;
	*s += (i - 1);
	if (to_find == NULL)
		return (NULL);
	target = (char *)to_find->data + (to_find->info + 1);
	return (target);
}

static char	*ft_trans_ev(char *s, t_lst *ev, char *ret)
{
	char	*tmp;
	int		flag;

	flag = 0;
	while (*s)
	{
		if (*s == '\'' && flag != F_DQUOTE)
			flag ^= F_QUOTE;
		if (*s == '\"' && flag != F_QUOTE)
			flag ^= F_DQUOTE;
		if (flag != F_QUOTE && *s == '$' && *(s + 1) != '?')
			tmp = ft_append(ret, ft_trans_ev_help(&s, ev), 0);
		else if (flag != F_QUOTE && *s == '$')
			tmp = ft_trans_ev_help2(&s, ret);
		else
			tmp = ft_append(ret, NULL, *s);
		free(ret);
		if (tmp == NULL)
			return (NULL);
		ret = tmp;
		s++;
	}
	return (ret);
}

static t_lst	*ft_trans_help(char *data, int info, t_lst *ev)
{
	char	*tmp;
	t_lst	*lst;
	t_lst	*ret;

	if (info == F_DEL)
	{
		tmp = ft_delete_quote(data, &info);
		if (tmp == NULL)
			return (NULL);
		ret = ft_heredoc(tmp, ((info & F_NO_TRANS) == 0), ev);
		free(tmp);
	}
	else
	{
		tmp = ft_trans_ev(data, ev, NULL);
		if (tmp == NULL)
			return (NULL);
		lst = ft_split_space(ft_trim(tmp));
		free(tmp);
		ret = ft_find_wild(lst, &info, NULL);
		ft_lst_free(lst);
	}
	return (ret);
}

static t_lst	*ft_trans(t_lst *target, t_lst *ev, t_lst *ret)
{
	t_lst	*to_push;

	while (target != NULL)
	{
		to_push = ft_trans_help(target->data, target->info, ev);
		if (to_push == NULL && target->info == F_DEL)
			return (ft_lst_free(ret));
		if (to_push != NULL)
		{
			if (ft_lst_size(to_push) != 1 && to_push->info != 0)
			{
				ft_error(F_ERROR_AMB, F_EXIT_STATUS_ARG);
				ft_lst_free(to_push);
				return (ft_lst_free(ret));
			}
			lst_push(&ret, to_push);
		}
		target = target->nxt;
	}
	return (ret);
}

int	ft_expansion(t_token *target, t_lst *ev)
{
	t_lst	*to_replace;

	if (target->argv != NULL)
	{
		to_replace = ft_trans(target->argv, ev, NULL);
		if (to_replace == NULL)
			return (1);
		ft_lst_free(target->argv);
		target->argv = to_replace;
	}
	if (target->redir != NULL)
	{
		to_replace = ft_trans(target->redir, ev, NULL);
		if (to_replace == NULL)
			return (1);
		ft_lst_free(target->redir);
		target->redir = to_replace;
	}
	return (0);
}
