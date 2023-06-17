/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_produce.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:44:45 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/17 16:13:21 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*mk_token(t_lst *target, t_lst *ev)
{
	t_token	*token;

	token = ft_calloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->fd[1] = 1;
	if (ft_split(token, target->data, " \t><", 0) != 0)
		return (ft_node_free(token, f_data_token));
	if (token->argv == NULL && token->redirection == NULL && target->size != 1)
	{
		ft_error(f_error_syntax);
		return (ft_node_free(token, f_data_token));
	}
	if (ft_expansion(token->argv, 0, ev) != 0)
		return (ft_node_free(token, f_data_token));
	if (ft_expansion(token->redirection, 1, ev) != 0)
		return (ft_node_free(token, f_data_token));
	if (token->argv != NULL \
			&& ft_resplit(token, token->argv, token->argv->size) != 0)
		return (ft_node_free(token, f_data_token));
	return (token);
}

t_lst	*mk_token_lst(char *line, t_lst *ev)
{
	t_lst	*line_lst;
	t_lst	*tmp;
	t_lst	*target;
	t_token	*token;

	line_lst = NULL;
	target = NULL;
	if (*line == '|')
		return (ft_lst_free(NULL, f_data_char, f_error_syntax));
	if (ft_split(&line_lst, line, "|", f_pipe) != 0)
		return (ft_lst_free(line_lst, f_data_char, NULL));
	tmp = line_lst;
	while (tmp != NULL)
	{
		token = mk_token(tmp, ev);
		if (token == NULL || lst_push(&target, mk_lst(token, 0)) != 0)
		{
			ft_node_free(token, f_data_token);
			ft_lst_free(line_lst, f_data_char, NULL);
			return (ft_lst_free(target, f_data_token, NULL));
		}
		tmp = tmp->nxt;
	}
	ft_lst_free(line_lst, f_data_char, NULL);
	return (target);
}

t_lst	*mk_file_lst(char *s, int dir_type)
{
	t_lst	*target;
	t_file	*f;

	f = ft_calloc(sizeof(t_file));
	if (f == NULL)
		return (NULL);
	f->file_name = mk_str_lst(s);
	if (f->file_name == NULL)
		return (ft_node_free(f, f_data_file));
	if ((dir_type & f_heredoc) != 0)
		dir_type &= ~f_input;
	if ((dir_type & f_append) != 0)
		dir_type &= ~f_output;
	f->mode = dir_type;
	target = mk_lst(f, 0);
	if (target == NULL)
		return (ft_node_free(f, f_data_file));
	return (target);
}

t_lst	*mk_str_lst(char *s)
{
	t_lst	*target;
	char	*data;

	target = NULL;
	while (*s)
	{
		data = ft_substr(s, 1);
		if (data == NULL || lst_push(&target, mk_lst(data, 0)) != 0)
		{
			free(data);
			return (ft_lst_free(target, f_data_string, NULL));
		}
		s++;
	}
	return (target);
}

t_lst	*mk_hash_lst(char *s)
{
	t_lst	*target_lst;
	t_hash	*target;
	int		i;

	i = 0;
	target = ft_calloc(sizeof(t_hash));
	if (target == NULL)
		return (NULL);
	while (ft_word_chk(s[i], "=", f_chk) != 0)
		i++;
	target->key = ft_substr(s, i);
	if (target->key == NULL)
		return (ft_node_free(target, f_data_hash));
	s += (i + 1);
	if (*s != '\0')
	{
		target->value = mk_str_lst(s);
		if (target->value == NULL)
			return (ft_node_free(target, f_data_hash));
	}
	target_lst = mk_lst(target, 0);
	if (target_lst == NULL)
		return (ft_node_free(target, f_data_hash));
	return (target_lst);
}
