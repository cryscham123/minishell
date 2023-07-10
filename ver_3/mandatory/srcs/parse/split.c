/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 09:09:14 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lst	*ft_lst_word_chk(t_lst **s, t_lst *data)
{
	t_lst	*tmp;
	int		flag;
	char	c;
	int		i;

	flag = 0;
	i = 0;
	tmp = *s;
	while (tmp != NULL && *(char *)tmp->data != '\0')
	{
		c = *(char *)tmp->data;
		if (c == '\'' && tmp->info == F_NO_PARSE && flag != F_DQUOTE)
			flag ^= F_QUOTE;
		if (c == '\"' && tmp->info == F_NO_PARSE && flag != F_QUOTE)
			flag ^= F_DQUOTE;
		if (flag == 0 && ft_str_find(" \t\n", c) != -1)
			break ;
		i++;
		tmp = tmp->nxt;
	}
	data = str_lst_dup(*s, i);
	(*s) = tmp;
	return (data);
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

t_lst	*ft_split_space_lst(t_lst *s)
{
	t_lst	*ret;
	t_lst	*to_push;
	t_lst	*data;

	ret = NULL;
	while (s != NULL && *(char *)s->data != '\0')
	{
		data = ft_lst_word_chk(&s, NULL);
		if (data == NULL)
			return (ft_lst_free(ret));
		to_push = mk_lst(data, F_DATA_STRING, 0);
		if (to_push == NULL)
		{
			ft_lst_free(data);
			return (ft_lst_free(ret));
		}
		lst_push(&ret, to_push);
		while (s != NULL && *(char *)s->data != '\0' \
			&& ft_str_find(" \t\n", *(char *)s->data) != -1)
			s = s->nxt;
	}
	return (ret);
}

t_lst	*ft_split_space(char *s)
{
	t_lst	*target;
	t_lst	*to_push;
	char	*data;

	target = NULL;
	while (*s)
	{
		data = ft_word_chk(&s, " \t\n");
		if (data == NULL)
			return (ft_lst_free(target));
		to_push = mk_lst(data, F_DATA_CHAR, 0);
		if (to_push == NULL)
		{
			free(data);
			return (ft_lst_free(target));
		}
		lst_push(&target, to_push);
		while (*s && (*s == ' ' || *s == '\t' || *s == '\n'))
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
