/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:57:48 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:18 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	sigint_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = 1;
}

void	heredoc_signal_handler(int sig)
{
	(void)sig;
	printf("\n");
	exit(1);
}

void	child_signal_handler(int sig)
{
	if (sig == SIGINT)
		write(2, "^C\n", 3);
	if (sig == SIGQUIT)
		write(2, "^\\Quit: 3\n", 10);
}

void	ft_signal(void (*handler_1)(int), void (*handler_2)(int))
{
	signal(SIGINT, handler_1);
	signal(SIGQUIT, handler_2);
}
