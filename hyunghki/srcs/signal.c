/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:57:48 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/21 16:01:51 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	sigint_handler(int sig)
{
	(void)sig;
	printf("\033[u\033[95mminishell$\033[0m ");
	printf("%s  \n", rl_line_buffer);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = 1;
}

void	heredoc_signal_handler(int sig)
{
	(void)sig;
	printf("\033[34mheredoc>\033[0m ");
	printf("%s  \n", rl_line_buffer);
	exit(1);
}

void	child_signal_handler(int sig)
{
	if (sig == SIGINT)
		write(2, "\n", 1);
	if (sig == SIGQUIT)
		write(2, "Quit: 3\n", 8);
}

void	ft_signal(void (*handler_1)(int), void (*handler_2)(int))
{
	signal(SIGINT, handler_1);
	signal(SIGQUIT, handler_2);
}
