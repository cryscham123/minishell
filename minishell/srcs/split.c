/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 14:38:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:18 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	meta_validate(char **s, int i, int *data_type)
{
	if ((*s)[i] == '|' && ((*data_type) & F_PIPE) == 0)
		(*data_type) |= F_PIPE;
	else if ((*s)[i] == '<' && ((*data_type) & F_OUTPUT) == 0)
	{
		if (((*data_type) & F_INPUT) == 0)
			(*data_type) |= F_INPUT;
		else if (((*data_type) & F_HEREDOC) == 0 && (*s)[i - 1] == '<')
			(*data_type) |= F_HEREDOC;
		else
			return (-1);
	}
	else if ((*s)[i] == '>' && ((*data_type) & F_INPUT) == 0)
	{
		if (((*data_type) & F_OUTPUT) == 0)
			(*data_type) |= F_OUTPUT;
		else if (((*data_type) & F_APPEND) == 0 && (*s)[i - 1] == '>')
			(*data_type) |= F_APPEND;
		else
			return (-1);
	}
	else if ((*s)[i] != ' ' && (*s)[i] != '\t')
		return (-1);
	return (1);
}

static int	ft_meta_chk(char **s, char *meta)
{
	int	i;
	int	data_type;

	i = 0;
	data_type = 0;
	while ((*s)[i] && !ft_word_chk((*s)[i], meta, F_CHK))
	{
		if (meta_validate(s, i, &data_type) < 0)
		{
			ft_word_chk(0, meta, F_RESET);
			return (-1);
		}
		i++;
	}
	if (data_type != 0 && !(*s)[i])
	{
		ft_word_chk(0, meta, F_RESET);
		return (-1);
	}
	(*s) += i;
	return (data_type);
}

static int	ft_inject_data(void *lst, char *s, int i, int d)
{
	char	*word;
	int		flag;

	if (i == 0 && d != 0)
		return (ft_error(F_ERROR_SYNTAX));
	if (i == 0)
		return (0);
	word = ft_substr(s, i);
	if (word == NULL)
		return (ft_error(F_ERROR_MEM));
	if (d == F_PIPE)
		flag = lst_push(lst, mk_lst(word, 0));
	else if ((d & F_INPUT) != 0 || (d & F_OUTPUT) != 0)
		flag = lst_push(&((t_token *)lst)->redirection, mk_file_lst(word, d));
	else
		flag = lst_push(&((t_token *)lst)->argv, mk_lst(mk_str_lst(word), 1));
	if (s[i] != '|' && d != F_PIPE)
		free(word);
	if (flag != 0)
		free(word);
	return (flag);
}

int	ft_word_chk(char c, char *meta, int mode)
{
	static int	quote_flag;

	if (mode == F_GET)
		return (quote_flag);
	if (mode == F_RESET)
	{
		quote_flag = 0;
		return (1);
	}
	if (mode == F_WORD)
	{
		if (c == '\'' && (quote_flag & F_DEQUOTE) == 0)
			quote_flag ^= F_QUOTE;
		if (c == '\"' && (quote_flag & F_QUOTE) == 0)
			quote_flag ^= F_DEQUOTE;
		if ((quote_flag & (F_QUOTE + F_DEQUOTE)) != 0)
			return (quote_flag);
	}
	while (*meta)
	{
		if (c == *meta)
			return (0);
		meta++;
	}
	return (1);
}

int	ft_split(void *target, char *s, char *meta, int flag)
{
	int		data_type;
	int		i;

	data_type = F_PIPE;
	while (*s)
	{
		i = 0;
		if (flag == 0)
			data_type = ft_meta_chk(&s, meta);
		else
			flag = 0;
		if (data_type < 0)
			return (ft_error(F_ERROR_SYNTAX));
		while (s[i] && ft_word_chk(s[i], meta, F_WORD))
			i++;
		if (ft_inject_data(target, s, i, data_type) != 0)
			return (ft_word_chk(0, meta, F_RESET));
		s += i;
	}
	if (ft_word_chk(0, meta, F_GET) != 0)
	{
		ft_word_chk(0, meta, F_RESET);
		return (ft_error(F_ERROR_SYNTAX));
	}
	return (0);
}
