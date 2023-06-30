/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:09:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 03:24:15 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_open(t_token *token)
{
	t_lst	*redir;
	int		fd;
	int		direction;

	redir = token->redir;
	if (redir->info == F_INPUT || redir->info == F_HEREDOC)
		fd = open(redir->data, O_RDONLY);
	else if (redir->info == F_OUTPUT)
		fd = open(redir->data, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd = open(redir->data, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
		return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
	direction = (redir->info == F_OUTPUT || redir->info == F_APPEND);
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
				return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
			((t_token *)tv->data)->fd[1] = fd_tmp[1];
			((t_token *)tv->nxt->data)->fd[0] = fd_tmp[0];
		}
		tv = tv->nxt;
	}
	return (0);
}
