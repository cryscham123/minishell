/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 14:38:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/13 14:03:36 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_word_chk(char c, char *meta, int mode)
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
			return (1);
	}
	while (*meta)
	{
		if (c == *meta)
			return (0);
		meta++;
	}
	return (1);
}

static int	dir_validate(char **s, int i, int *dir)
{
	if ((*s)[i] == '|' && ((*dir) & f_pipe) == 0)
		(*dir) |= f_pipe;
	else if ((*s)[i] == '<' && ((*dir) & f_output) == 0)
	{
		if (((*dir) & f_input) == 0)
			(*dir) |= f_input;
		else if (((*dir) & f_heredoc) == 0 && (*s)[i - 1] == '<')
			(*dir) |= f_heredoc;
		else
			return (-1);
	}
	else if ((*s)[i] == '>' && ((*dir) & f_input) == 0)
	{
		if (((*dir) & f_output) == 0)
			(*dir) |= f_output;
		else if (((*dir) & f_appand) == 0 && (*s)[i - 1] == '>')
			(*dir) |= f_appand;
		else
			return (-1);
	}
	else if ((*s)[i] != ' ' && (*s)[i] != '\t')
		return (-1);
	return (1);
}

static int	ft_dir_chk(char **s, char *meta)
{
	int	i;
	int	dir;

	i = 0;
	dir = 0;
	while ((*s)[i] && !ft_word_chk((*s)[i], meta, f_dir))
	{
		if (dir_validate(s, i, &dir) < 0)
			return (-1);
		i++;
	}
	if (dir != 0 && !(*s)[i])
		return (-1);
	(*s) += i;
	return (dir);
}

static int	ft_split_push_back(void *target, char *s, int i, int dir_chk)
{
	char	*word;
	t_token	*tmp;

	if (i == 0)
		return (dir_chk != 0);
	word = ft_substr(s, i);
	if (word == NULL)
		return (1);
	if (s[i] == '|' || dir_chk == f_pipe)
		return (lst_push(target, mk_lst(word, 0, 0)));
	tmp = target;
	if ((dir_chk & f_input) != 0)
	{
		if ((dir_chk & f_heredoc) != 0)
			return (lst_push(&tmp->redirection, mk_lst(word, 2, f_heredoc)));
		return (lst_push(&tmp->redirection, mk_lst(word, 2, f_input)));
	}
	if ((dir_chk & f_output) != 0)
	{
		if ((dir_chk & f_appand) != 0)
			return (lst_push(&tmp->redirection, mk_lst(word, 2, f_appand)));
		return (lst_push(&tmp->redirection, mk_lst(word, 2, f_output)));
	}
	return (lst_push(&tmp->argv, mk_lst(word, 0, 0)));
}

int	ft_split(void *target, char *s, char *meta)
{
	int		dir_chk;
	int		i;

	while (*s)
	{
		i = 0;
		dir_chk = ft_dir_chk(&s, meta);
		if (dir_chk < 0)
			return (ft_word_chk(0, meta, f_reset));
		while (s[i] && ft_word_chk(s[i], meta, f_word))
			i++;
		if (ft_split_push_back(target, s, i, dir_chk) != 0)
			return (ft_word_chk(0, meta, f_reset));
		s += i;
	}
	if (ft_word_chk(0, meta, f_get) != 0)
		return (ft_word_chk(0, meta, f_reset));
	return (0);
}
