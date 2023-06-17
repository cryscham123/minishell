/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/17 16:11:47 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

static void	ft_exe(t_lst *tv, t_lst *ev)
{
	t_token	*data;

	while (tv != NULL)
	{
		data = tv->data;
		if (ft_redirection(data, data->redirection, ev) != 0)
		{
			tv = tv->nxt;
			continue ;
		}
		dup2(data->fd[0], 0);
		dup2(data->fd[1], 1);
		if (data->argv == NULL)
			return ;
		//g_status = ft_cmd(data->argv, ev, (tv->size == 1));
		tv = tv->nxt;
	}
}

static void	*ft_parse(t_lst *ev)
{
	char		*line;
	t_lst		*tv;

	tv = NULL;
	line = readline("\033[95mminishell$\033[0m ");
	if (line != NULL && *line)
	{
		add_history(line);
		tv = mk_token_lst(line, ev);
		if (tv == NULL)
		{
			free(line);
			return (NULL);
		}
		ft_exe(tv, ev);
		ft_lst_free(tv, f_data_token, NULL);
	}
	free(line);
	return (NULL);
}

static void	*mk_ev(char **env)
{
	t_lst	*ev;

	ev = NULL;
	if (lst_push(&ev, mk_hash_lst("=$")) != 0)
		return (ft_lst_free(ev, f_data_hash, NULL));
	while (*env)
	{
		if (lst_push(&ev, mk_hash_lst(*env)) != 0)
			return (ft_lst_free(ev, f_data_hash, NULL));
		env++;
	}
	return (ev);
}

int	main(int argc, char **argv, char **env)
{
	t_lst		*ev;

	(void)argv;
	if (argc != 1)
		return (ft_error(f_error_arg));
	// signal handle
	ev = mk_ev(env);
	if (ev == NULL)
		return (1);
	while (1)
		ft_parse(ev);
	return (0);
}
