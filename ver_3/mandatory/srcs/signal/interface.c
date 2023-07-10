/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:57:48 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 08:19:49 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	sigint_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = 1;
}

void	heredoc_signal_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 2);
	exit(1);
}

int	cal_flag(int ret_val)
{
	if (ret_val == 0 || ret_val == 1)
		return (ret_val);
	if (ret_val == F_EXIT_STATUS_BUILTIN)
		return (1);
	if (WIFEXITED(ret_val))
		return (WEXITSTATUS(ret_val));
	if (ret_val == 3)
		ft_putstr_fd("Quit: 3\n", 2);
	return (ret_val + 128);
}

void	ft_signal(void (*handler_1)(int), void (*handler_2)(int), int flag)
{
	struct termios	term;	

	tcgetattr(STDIN_FILENO, &term);
	if (flag == 1)
		term.c_lflag |= ECHOCTL;
	else
		term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, handler_1);
	signal(SIGQUIT, handler_2);
}
