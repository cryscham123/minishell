/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

static void	*ft_parse(t_lst *ev)
{
	char	*line;
	t_lst	*tv;

	tv = NULL;
	line = readline("\033[s\033[95mminishell$\033[0m ");
	if (line == NULL)
	{
		printf("\033[u\033[95mminishell$\033[0m exit\n");
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
	struct termios	term;
	t_lst			*ev;

	(void)argv;
	if (argc != 1)
		return (ft_error(F_ERROR_ARG));
	ev = mk_ev(env);
	if (ev == NULL)
		return (1);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	while (1)
	{
		ft_signal(sigint_handler, SIG_IGN);
		ft_parse(ev);
	}
	ft_lst_free(ev, F_DATA_HASH, NULL);
	return (0);
}
