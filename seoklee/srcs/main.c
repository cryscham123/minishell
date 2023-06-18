/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/18 18:49:07 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

static void	*ft_parse(t_lst *ev, char **env)
{
	char	*line;
	t_lst	*tv;

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
		g_status = ft_exe(tv, ev, env);
		ft_lst_free(tv, F_DATA_TOKEN, NULL);
	}
	free(line);
	return (NULL);
}

static void	*mk_ev(char **env)
{
	t_lst	*ev;

	ev = NULL;
	if (lst_push(&ev, mk_hash_lst("=$")) != 0)
		return (ft_lst_free(ev, F_DATA_HASH, F_ERROR_MEM));
	while (*env)
	{
		if (lst_push(&ev, mk_hash_lst(*env)) != 0)
			return (ft_lst_free(ev, F_DATA_HASH, F_ERROR_MEM));
		env++;
	}
	return (ev);
}

int	main(int argc, char **argv, char **env)
{
	t_lst		*ev;

	(void)argv;
	if (argc != 1)
		return (ft_error(F_ERROR_ARG));
	ev = mk_ev(env);
	if (ev == NULL)
		return (1);
	while (1)
		ft_parse(ev, env);
	return (0);
}