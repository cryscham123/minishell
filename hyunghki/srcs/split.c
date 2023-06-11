/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 14:38:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/11 15:47:11 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_free_all(char **target, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(target[i]);
		i++;
	}
	free(target);
	return (NULL);
}

static int	is_meta(char c, char *meta)
{
	static int	is_quote;

	if (c == '\'')
	{
		if (is_quote == 0)
			is_quote = 1;
		else if (is_quote == 1)
			is_quote = 0;
	}
	else if (c == '\"')
	{
		if (is_quote == 0)
			is_quote = 2;
		else if (is_quote == 2)
			is_quote = 0;
	}
	if (is_quote != 0)
		return (2);
	while (*meta)
	{
		if (c == *meta)
			return (1);
		meta++;
	}
	return (0);
}

static int	chk_word_cnt(char *s, char *meta)
{
	int	cnt;
	int	i;
	
	cnt = 0;
	while (*s)
	{
		i = 0;
		while (s[i] && is_meta(s[i], meta) != 1)
			i++;
		if (s[i] && ((!s[i + 1]) || is_meta(s[i + 1], meta) == 1))
			return (-1);
		if (i > 0)
			cnt++;
		s += (i + (i == 0));
	}
	if (is_meta(*s, meta) == 2)
		return (-1);
	return (cnt);
}

char	**ft_split(char **s, char *meta)
{
	char	**target;
	int		i;
	int		j;
	int		cnt;

	cnt = chk_word_cnt(*s, meta);
	if (cnt < 0)
		return (NULL);
	target = ft_calloc(sizeof(char *) * (cnt + 1));
	j = 0;
	while (target != NULL && j < cnt)
	{
		i = 0;
		while ((*s)[i] && is_meta((*s)[i], meta) != 1)
			i++;
		if (i > 0)
		{
			target[j] = ft_calloc(i + 1);
			if (target[j] == NULL)
				return (ft_free_all(target, j));
			ft_strncpy(target[j++], *s, i);
		}
		(*s) += (i + (i == 0));
	}
	return (target);
}
