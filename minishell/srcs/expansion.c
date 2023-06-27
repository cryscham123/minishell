/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 12:14:14 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:23 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	mov_cur(t_lst **cur, t_lst *to_link, t_lst *tmp)
{
	t_lst	*to_del;

	to_del = (*cur)->nxt;
	if (to_link != NULL)
	{
		(*cur)->nxt = to_link;
		(*cur) = ft_lst_back(to_link);
	}
	if (*cur != tmp->nxt)
	{
		(*cur)->nxt = tmp->nxt;
		tmp->nxt = NULL;
		ft_lst_free(to_del, NULL, F_DATA_CHAR, NULL);
	}
	else
		(*cur)->nxt = to_del;
}

static int	trans_help(t_lst **target, t_lst **tmp, t_lst *cur, t_lst *ev)
{
	char	*to_find;
	t_lst	*to_link;
	int		i;

	i = 0;
	*tmp = cur;
	while ((*tmp)->nxt != NULL \
		&& ft_word_chk(*(char *)(*tmp)->nxt->data, "| \t><$\'\"=", F_CHK) != 0)
	{
		*tmp = (*tmp)->nxt;
		i++;
	}
	to_find = ft_c_str(cur->nxt, NULL, 0, i);
	if (to_find == NULL)
		return (ft_error(F_ERROR_MEM));
	to_link = ft_hash_find(ev, to_find);
	free(to_find);
	if (to_link != NULL && dup_str_lst(&to_link, to_link) != 0)
		return (1);
	*target = to_link;
	return (0);
}

static int	ft_translate(t_lst **cur, t_lst *ev)
{
	char	*c_tmp;
	t_lst	*to_link;
	t_lst	*to_del;
	t_lst	*tmp;

	to_del = (*cur);
	*(char *)(*cur)->data = '\0';
	if (to_del->nxt != NULL && *(char *)to_del->nxt->data == '?')
	{
		c_tmp = ft_itoa(g_status);
		if (c_tmp == NULL)
			return (ft_error(F_ERROR_MEM));
		to_link = mk_str_lst(c_tmp);
		free(c_tmp);
		if (to_link == NULL)
			return (ft_error(F_ERROR_MEM));
		tmp = to_del->nxt;
	}
	else if (trans_help(&to_link, &tmp, to_del, ev) != 0)
		return (ft_error(F_ERROR_MEM));
	mov_cur(cur, to_link, tmp);
	return (0);
}

static int	exp_help(t_lst *cur, t_lst *ev, t_file *file_info, int is_heredoc)
{
	char	val;
	int		flag;

	flag = 0;
	while (cur != NULL)
	{
		val = *(char *)cur->data;
		if ((flag == F_QUOTE && val == '\'') \
			|| (flag == F_DQUOTE && val == '\"'))
			*(char *)cur->data = '\0';
		flag = ft_word_chk(val, "", F_WORD);
		if ((flag == F_QUOTE && val == '\'') \
			|| (flag == F_DQUOTE && val == '\"'))
		{
			*(char *)cur->data = '\0';
			if (is_heredoc)
				file_info->mode |= F_NO_TRANS;
		}
		if (!is_heredoc && flag != F_QUOTE && val == '$' \
			&& ft_translate(&cur, ev) != 0)
			return (ft_word_chk(0, "", F_RESET));
		if (cur != NULL)
			cur = cur->nxt;
	}
	return (0);
}

int	ft_expansion(t_lst *lst, int is_redir, t_lst *ev)
{
	t_lst		*tmp;
	int			is_heredoc;
	t_file		*file_info;

	tmp = lst;
	while (tmp != NULL)
	{
		if (is_redir == 0 && exp_help(tmp->data, ev, NULL, 0) != 0)
			return (1);
		if (is_redir != 0)
		{
			file_info = tmp->data;
			is_heredoc = (file_info->mode == F_HEREDOC);
			if (exp_help(file_info->file_name, ev, file_info, is_heredoc) != 0)
				return (1);
			if (is_heredoc && ft_heredoc(file_info, ev, NULL) != 0)
				return (1);
		}
		tmp = tmp->nxt;
	}
	return (0);
}
