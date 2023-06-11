/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:23:00 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/11 15:28:24 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static t_token	*mk_token(char **token_arr)
{
	// word devide
	// word expansion
	// redirection info handle
}

static void ft_token_push_back(t_token_v *tv, t_token *token)
{
	if (tv->cur_token == NULL)
		tv->cur_token = token;
	else
		tv->cur_token->nxt_token = token;
	tv->cur_token_len++;
}

int	ft_parse_data(t_token_v *tv, char *target)
{
	char		**token_arr;
	t_token		*token;

	while (*target)
	{
		token_arr = ft_split(&target, "|");
		if (token_arr == NULL)
			// error
		while (token_arr)
		{
			token = mk_token(token_arr);
			if (token == NULL)
				// error
			ft_token_push_back(tv, token);
			token_arr++;
		}
	}
	return (0);
}
