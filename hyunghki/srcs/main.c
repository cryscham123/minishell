/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/20 16:14:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	g_status;

static void	*ft_parse(t_lst *ev)
{
	char	*line;
	t_lst	*tv;

	tv = NULL;
	line = readline("\033[95mminishell$\033[0m ");
	if (line == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (*line)
	{
		add_history(line);
		tv = mk_token_lst(line, ev);
		if (tv == NULL)
		{
			free(line);
			return (NULL);
		}
		g_status = ft_exe(tv, ev, NULL, ft_str_size(tv));
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
	((t_hash *)ev->data)->env = env;
	while (*env)
	{
		if (lst_push(&ev, mk_hash_lst(*env)) != 0)
			return (ft_lst_free(ev, F_DATA_HASH, F_ERROR_MEM));
		env++;
	}
	return (ev);
}

void	handle_signal(int sig)
{
	if (sig == SIGQUIT)
	{
		if (g_status == F_STATUS_ONGOING)
			printf("Quit: %d\n", SIGQUIT);
		else
		{
			rl_on_new_line();
			rl_redisplay();
		}
	}
	else if (sig == SIGINT)
	{
		if (g_status == F_STATUS_ONGOING)
			printf("\n");
		else
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		g_status = 1;
	}
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
	{
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, handle_signal);
		ft_parse(ev);
	}
	ft_lst_free(ev, F_DATA_HASH, NULL);
	return (0);
}
