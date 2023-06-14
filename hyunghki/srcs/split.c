/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 14:38:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/14 18:24:14 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	meta_validate(char **s, int i, int *data_type)
{
	if ((*s)[i] == '|' && ((*data_type) & f_pipe) == 0)
		(*data_type) |= f_pipe;
	else if ((*s)[i] == '<' && ((*data_type) & f_output) == 0)
	{
		if (((*data_type) & f_input) == 0)
			(*data_type) |= f_input;
		else if (((*data_type) & f_heredoc) == 0 && (*s)[i - 1] == '<')
			(*data_type) |= f_heredoc;
		else
			return (-1);
	}
	else if ((*s)[i] == '>' && ((*data_type) & f_input) == 0)
	{
		if (((*data_type) & f_output) == 0)
			(*data_type) |= f_output;
		else if (((*data_type) & f_appand) == 0 && (*s)[i - 1] == '>')
			(*data_type) |= f_appand;
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
	while ((*s)[i] && !ft_word_chk((*s)[i], meta, f_chk))
	{
		if (meta_validate(s, i, &data_type) < 0)
		{
			ft_word_chk(0, meta, f_reset);
			return (-1);
		}
		i++;
	}
	if (data_type != 0 && !(*s)[i])
	{
		ft_word_chk(0, meta, f_reset);
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
		return (ft_error(f_error_syntax));
	if (i == 0)
		return (0);
	word = ft_substr(s, i);
	if (word == NULL)
		return (ft_error(f_error_mem));
	if (d == f_pipe)
		flag = lst_push(lst, mk_lst(word, 0));
	else if ((d & f_input) != 0 || (d & f_output) != 0)
		flag = lst_push(&((t_token *)lst)->redirection, mk_file_lst(word, d));
	else
		flag = lst_push(&((t_token *)lst)->argv, mk_lst(mk_str_lst(word), 1));
	if (s[i] != '|' && d != f_pipe)
		free(word);
	if (flag != 0)
		free(word);
	return (flag);
}

int	ft_word_chk(char c, char *meta, int mode)
{
	static int	quote_flag;

	if (mode == f_get)
		return (quote_flag);
	if (mode == f_reset)
	{
		quote_flag = 0;
		return (1);
	}
	if (mode == f_word)
	{
		if (c == '\'' && (quote_flag & f_dequote) == 0)
			quote_flag ^= f_quote;
		if (c == '\"' && (quote_flag & f_quote) == 0)
			quote_flag ^= f_dequote;
		if ((quote_flag & (f_quote + f_dequote)) != 0)
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

	data_type = f_pipe;
	while (*s)
	{
		i = 0;
		if (flag == 0)
			data_type = ft_meta_chk(&s, meta);
		else
			flag = 0;
		if (data_type < 0)
			return (ft_error(f_error_syntax));
		while (s[i] && ft_word_chk(s[i], meta, f_word))
			i++;
		if (ft_inject_data(target, s, i, data_type) != 0)
			return (ft_word_chk(0, meta, f_reset));
		s += i;
	}
	if (ft_word_chk(0, meta, f_get) != 0)
	{
		ft_word_chk(0, meta, f_reset);
		return (ft_error(f_error_syntax));
	}
	return (0);
}
