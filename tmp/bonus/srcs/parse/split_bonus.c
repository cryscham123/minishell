/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:34:10 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	ft_flag_chk(char **s, int *flag)
{
	if (*flag != 0)
	{
		*flag = 0;
		return (ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX));
	}
	if (**s == '|')
		*flag = F_PIPE;
	if (**s == '>')
	{
		if (*(*s + 1) == '>')
			*flag = F_APPEND;
		else
			*flag = F_OUTPUT;
	}
	if (**s == '<')
	{
		if (*(*s + 1) == '<')
			*flag = F_DEL;
		else
			*flag = F_INPUT;
	}
	(*s) += 1 + (*flag == F_APPEND || *flag == F_DEL);
	return (0);
}

static char	*ft_word_chk(char **s, char *meta)
{
	int		i;
	int		quote_flag;
	char	*data;

	quote_flag = 0;
	i = -1;
	while ((*s)[++i])
	{
		if ((*s)[i] == '\'' && quote_flag != F_DQUOTE)
			quote_flag ^= F_QUOTE;
		if ((*s)[i] == '\"' && quote_flag != F_QUOTE)
			quote_flag ^= F_DQUOTE;
		if (quote_flag == 0 && ft_str_find(meta, (*s)[i]) != -1)
			break ;
	}
	if (quote_flag != 0)
	{
		ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX);
		return (NULL);
	}
	data = ft_substr(*s, i);
	(*s) += i;
	return (data);
}

t_lst	*ft_split_space(char *s)
{
	t_lst	*target;
	t_lst	*to_push;
	char	*data;

	target = NULL;
	while (*s)
	{
		data = ft_word_chk(&s, " \t");
		if (data == NULL)
			return (ft_lst_free(target));
		to_push = mk_lst(data, F_DATA_CHAR, 0);
		if (to_push == NULL)
		{
			free(data);
			return (ft_lst_free(target));
		}
		lst_push(&target, to_push);
		while (*s && (*s == ' ' || *s == '\t'))
			s++;
	}
	return (target);
}

int	ft_split(char *s, char *meta, int *flag, t_lst **target)
{
	char	*data;
	t_lst	*to_push;

	while (*s)
	{
		if (ft_str_find(meta, *s) != -1 && ft_flag_chk(&s, flag) != 0)
			return (1);
		if (*s != '\0' && ft_str_find(meta, *s) == -1)
		{
			data = ft_word_chk(&s, meta);
			if (data == NULL)
				return (1);
			to_push = mk_lst(data, F_DATA_CHAR, *flag);
			if (to_push == NULL)
			{
				free(data);
				return (1);
			}
			lst_push(target, to_push);
			*flag = 0;
		}
	}
	return (0);
}
