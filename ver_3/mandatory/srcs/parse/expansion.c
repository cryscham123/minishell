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

static t_lst	*ft_trans_ev_help(char *s, t_lst *ev, int flag, t_lst *ret)
{
	t_lst	*to_push;

	while (*s)
	{
		if (*s == '\'' && flag != F_DQUOTE)
		{
			flag ^= F_QUOTE;
			to_push = mk_str_node(s, 1, F_NO_PARSE);
		}
		else if (*s == '\"' && flag != F_QUOTE)
		{
			flag ^= F_DQUOTE;
			to_push = mk_str_node(s, 1, F_NO_PARSE);
		}
		else if (flag != F_QUOTE && *s == '$' && *(s + 1) == '?')
			to_push = ft_trans_status(&s);
		else if (flag != F_QUOTE && *s == '$')
			to_push = ft_trans_dollar(&s, ev, flag);
		else
			to_push = mk_str_node(s, 1, 0);
		lst_push(&ret, to_push);
		s++;
	}
	return (ret);
}

static t_lst	*ft_trans_ev(char *data, int info, t_lst *ev)
{
	t_lst	*tmp;
	t_lst	*splitted;
	t_lst	*ret;

	tmp = ft_trans_ev_help(data, ev, 0, NULL);
	if (tmp == NULL)
		return (NULL);
	splitted = ft_split_space_lst(ft_lst_trim(tmp));
	ft_lst_free(tmp);
	if (splitted == NULL)
		return (NULL);
	ret = ft_find_wild(splitted, info, NULL);
	ft_lst_free(splitted);
	return (ret);
}

static t_lst	*ft_trans_help(char *data, int info, t_lst *ev)
{
	char	*tmp;
	t_lst	*ret;

	if (info == F_DEL)
	{
		tmp = ft_delete_quote(data, &info);
		if (tmp == NULL)
			return (NULL);
		ret = ft_heredoc(tmp, ((info & F_NO_TRANS) == 0), ev);
		free(tmp);
		return (ret);
	}
	return (ft_trans_ev(data, info, ev));
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
