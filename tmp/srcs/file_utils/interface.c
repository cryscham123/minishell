/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:09:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_open(t_token *token)
{
	int	fd;
	int	direction;

	if (token->info == F_INPUT || token->info == F_HEREDOC)
		fd = open(token->data, O_RDONLY);
	else if (token->info == F_OUTPUT)
		fd = open(token->data, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd = open(token->data, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
		return (ft_error(F_ERROR_FILE, F_EXIT_STATUS_FILE));
	direction = (token->info == F_OUTPUT || token->info == F_APPEND);
	if (token->fd[direction] != direction)
		close(token->fd[direction]);
	token->fd[direction] = fd;
	return (0);
}

void	ft_close(int *fd, t_lst *tv)
{
	if (fd[0] != 0)
		close(fd[0]);
	if (fd[1] != 1)
		close(fd[1]);
	while (tv != NULL)
	{
		ft_close(((t_token *)tv->data)->fd, NULL);
		tv = tv->nxt;
	}
}

void	ft_dup2(int *fd, t_lst *tv)
{
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	ft_close(fd, tv);
}

int	ft_pipe(t_lst *tv)
{
	int	fd_tmp[2];

	while (tv != NULL)
	{
		if (tv->nxt != NULL)
		{
			if (pipe(fd_tmp) < 0)
				return (ft_error(F_ERROR_FILE));
			((t_token *)tv->data)->fd[1] = fd_tmp[1];
			((t_token *)tv->nxt->data)->fd[0] = fd_tmp[0];
		}
		tv = tv->nxt;
	}
	return (0);
}
