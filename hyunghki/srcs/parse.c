/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:23:00 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/13 14:17:58 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_expansion(t_lst *lst, t_lst *ev)
{

}

static t_token	*mk_token(t_lst *target, t_lst *ev)
{
	t_token	*token;

	token = ft_calloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	if (ft_split(token, target->data, " \t><") != 0)
	{
		ft_lst_free(token->argv, 0);
		return (ft_lst_free(token->redirection, 2));
	}
	if (token->argv == NULL && token->redirection == NULL && target->size != 1)
	{
		ft_lst_free(token->argv, 0);
		return (ft_lst_free(token->redirection, 2));
	}
	if (ft_expansion(token->argv, ev) == 0 \
		&& ft_expansion(token->redirection, ev) == 0)
		return (token);
	ft_lst_free(token->argv, 0);
	return (ft_lst_free(token->redirection, 2));
}

static t_lst	*ft_produce_token(char *line, t_lst *ev)
{
	t_token	*token;
	t_lst	*line_lst;
	t_lst	*target;
	t_lst	*tmp;

	line_lst = NULL;
	if (*line == '|' || ft_split(&line_lst, line, "|") != 0)
		return (ft_lst_free(line_lst, 0));
	target = NULL;
	tmp = line_lst;
	while (tmp != NULL)
	{
		token = mk_token(tmp, ev);
		if (token == NULL || lst_push(&target, mk_lst(token, 1, 0)) != 0)
		{
			ft_lst_free(target, 1);
			return (ft_lst_free(line_lst, 0));
		}
		tmp = tmp->nxt;
	}
	ft_lst_free(line_lst, 0);
	return (target);
}

static void	ft_test(t_lst *tv, t_lst *ev)
{
	(void)ev;
	while (tv != NULL)
	{
		t_token *data = tv->data;
		printf("arguments: ");
		while (data->argv != NULL)
		{
			printf("%s ", data->argv->data);
			data->argv = data->argv->nxt;
		}
		printf("\nredirection: ");
		while (data->redirection != NULL)
		{
			t_file *f = data->redirection->data;
			printf("%s ", f->file_name);
			data->redirection = data->redirection->nxt;
		}
		printf("\n");
		tv = tv->nxt;
	}
}

void	ft_parse(t_lst *ev)
{
	char		*line;
	t_lst		*tv;
	
	tv = NULL;
	line = readline("$ ");
	if (line != NULL && *line)
	{
		add_history(line);
		tv = ft_produce_token(line, ev);
		if (tv == NULL)
			printf("Error ocurred...\n");
		else
		{
			ft_text(tv, ev);
			// ft_exe(tv, ev);
			ft_lst_free(tv, 1);
		}
	}
	free(line);
}
