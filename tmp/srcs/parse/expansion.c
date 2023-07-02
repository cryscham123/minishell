/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 05:40:45 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_replace_ev_help(t_lst **target, t_lst *ev)
{
	t_lst	*val_str;
	t_lst	*to_find;
	char	*val;

	to_find = (*target)->nxt;
	while (to_find != NULL \
		&& ft_str_find("| \t><$\'\"=*", *(char *)to_find->data) == -1)
		to_find = to_find->nxt;
	if (to_find != NULL && to_find->prev != NULL)
		to_find->prev->nxt = NULL;
	val = ft_env_find_lst(ev, target->nxt);
	if (to_find != NULL && to_find->prev != NULL)
		to_find->prev->nxt = to_find;
	if (val == NULL)
	{
		lst_replace(*target, to_find, NULL, NULL);
		return (0);
	}
	val_str = mk_str_lst(val);
	if (val_str == NULL)
		return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
	lst_replace(*target, to_find, val_str, lst_back(val_str));
	return (0);
}

static int	ft_replace_ev(t_lst *target, t_lst *ev, int *info)
{
	char	c;
	int		flag;

	flag = 0;
	while (target != NULL)
	{
		c = *(char *)target->data;
		if (c == '\'' && flag != F_DQUOTE)
		{
			target->info = F_NO_PARSE;
			(*info) |= (*info == F_DEL) * F_NO_TRANS;
			flag ^= F_QUOTE;
		}
		if (c == '\"' && flag != F_QUOTE)
		{
			target->info = F_NO_PARSE;
			(*info) |= (*info == F_DEL) * F_NO_TRANS;
			flag ^= F_DQUOTE;
		}
		if (*info != F_DEL && flag != F_QUOTE && c == '$' \
			&& ft_replace_ev_help(&target, ev) != 0)
			return (1);
		target = target->nxt;
	}
	return (0);
}

static t_lst	*ft_trans_help(t_lst *target, t_lst *ev, int *info)
{
	char	*tmp;
	t_lst	*ret;

	if (ft_replace_ev(target, ev, info) != 0)
		return (NULL);
	if (((*info) & F_DEL) != 0)
	{
		tmp = ft_c_str(target);
		if (tmp == NULL)
		{
			ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM);
			return (NULL);
		}
		ret = ft_heredoc(tmp, ((*info & F_NO_TRANS) == 0), ev);
		free(tmp);
	}
	else
		ret = ft_lst_split_space(ft_lst_trim(target));
	ft_lst_free(target);
	return (ret);
}

static t_lst	*ft_trans(t_lst *target, t_lst *ev, t_lst *ret)
{
	t_lst	*data;
	t_lst	*to_push;

	while (target != NULL)
	{
		data = mk_str_lst(target->data);
		if (data == NULL)
		{
			ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM);
			return (ft_lst_free(ret));
		}
		to_push = ft_trans_help(data, ev, &target->info);
		if (to_push == NULL)
			return (ft_lst_free(ret));
		if (ft_lst_size(to_push) != 1 && to_push->info != 0)
		{
			ft_error(F_ERROR_AMB, F_EXIT_STATUS_ARG);
			ft_lst_free(to_push);
			return (ft_lst_free(ret));
		}
		target = target->nxt;
		lst_push(&ret, to_push);
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
