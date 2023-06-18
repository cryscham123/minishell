/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 12:14:14 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/18 18:33:07 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static int	trans_help(t_lst **target, t_lst **lst, t_lst *cur, t_lst *ev)
{
	char	*to_find;
	t_lst	*to_link;
	t_lst	*tmp;
	int		i;

	tmp = cur;
	i = 0;
	while (tmp->nxt != NULL \
			&& ft_word_chk(*(char *)tmp->nxt->data, "| \t><$\'\"=", F_CHK) != 0)
	{
		tmp = tmp->nxt;
		i++;
	}
	to_find = ft_c_str(cur->nxt, NULL, i, 0);
	to_link = ft_hash_find(ev, to_find);
	free(to_find);
	if (to_link != NULL && dup_str_lst(&to_link, to_link) != 0)
		return (1);
	if (to_link != NULL)
		(*lst) = ft_lst_back(to_link);
	if (tmp->nxt != NULL)
		lst_push(&to_link, tmp->nxt);
	tmp->nxt = NULL;
	*target = to_link;
	return (0);
}

static int	trs(t_lst **cur, t_lst **prev, t_lst **lst, t_lst *ev)
{
	t_lst	*to_link;
	t_lst	*tmp;

	tmp = *cur;
	if (tmp->nxt != NULL && *(char *)tmp->nxt->data == '?')
	{
		to_link = ft_itoa(g_status);
		if (to_link == NULL)
			return (ft_error(F_ERROR_MEM));
		(*cur) = ft_lst_back(to_link);
		if (tmp->nxt->nxt != NULL)
			lst_push(&to_link, tmp->nxt->nxt);
		tmp->nxt->nxt = NULL;
	}
	else if (trans_help(&to_link, cur, tmp, ev) != 0)
			return (ft_error(F_ERROR_MEM));
	ft_lst_free(tmp, F_DATA_CHAR, NULL);
	if (*prev == NULL)
		*lst = to_link;
	else
		(*prev)->nxt = to_link;
	if (to_link == NULL)
		*cur = NULL;
	return (0);
}

static int	exp_help(t_lst **lst, t_lst *ev, t_lst *prev)
{
	int		flag;
	t_lst	*cur;

	cur = *lst;
	while (cur != NULL)
	{
		flag = ft_word_chk(*(char *)cur->data, "$", F_WORD);
		if (flag == F_DEQUOTE)
		{
			while (ft_word_chk(*(char *)cur->data, "\"", F_CHK) != 0)
			{
				if (*(char *)cur->data == '$' && trs(&cur, &prev, lst, ev) != 0)
					return (ft_word_chk(0, "", F_RESET));
				prev = cur;
				cur = cur->nxt;
			}
			ft_word_chk(0, "", F_RESET);
		}
		else if (flag == 0 && trs(&cur, &prev, lst, ev) != 0)
			return (ft_word_chk(0, "", F_RESET));
		prev = cur;
		if (cur != NULL)
			cur = cur->nxt;
	}
	return (0);
}

int	ft_resplit(t_token *token, t_lst *lst, int n)
{
	t_lst	*tmp;
	char	*target;
	int		i;

	tmp = lst;
	i = 0;
	while (1)
	{
		target = ft_c_str(tmp->data, NULL, -1, 0);
		if (target == NULL)
			return (ft_error(F_ERROR_MEM));
		if (ft_split(token, target, " \t", 0) != 0)
			return (1);
		free(target);
		if (++i == n)
			break ;
		tmp = tmp->nxt;
	}
	token->argv = tmp->nxt;
	tmp->nxt = NULL;
	ft_lst_free(lst, F_DATA_STRING, NULL);
	return (0);
}

int	ft_expansion(t_lst *lst, int is_redir, t_lst *ev)
{
	static int	heredoc_name;
	t_lst		*tmp;
	t_file		*file_info;

	tmp = lst;
	while (tmp != NULL)
	{
		if (is_redir == 0 && exp_help((t_lst **)&tmp->data, ev, NULL) != 0)
			return (1);
		if (is_redir != 0)
		{
			file_info = tmp->data;
			if (file_info->mode == F_HEREDOC)
			{
				if (ft_heredoc(file_info, ++heredoc_name, ev) != 0)
					return (1);
			}
			else if (exp_help(&file_info->file_name, ev, NULL) != 0)
				return (1);
		}
		tmp = tmp->nxt;
	}
	return (0);
}
