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

static int	ft_open(t_token *token, char *s, int mode)
{
	int	fd;
	int	direction;

	if (mode == F_INPUT || mode == F_HEREDOC)
		fd = open(s, O_RDONLY);
	else if (mode == F_OUTPUT)
		fd = open(s, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd = open(s, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		free(s);
		return (ft_error(F_ERROR_FILE));
	}
	direction = (mode == F_OUTPUT || mode == F_APPEND);
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

void	ft_unlink(t_lst *file_name)
{
	char	*name;

	name = ft_c_str(file_name, NULL, -1, 0);
	unlink(name);
	free(name);
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

int	ft_redirection(t_token *token, t_lst *redir)
{
	t_file	*data;
	t_lst	*tmp;
	char	*file_name;

	while (redir != NULL)
	{
		data = redir->data;
		tmp = data->file_name;
		while (tmp != NULL)
		{
			if (ft_word_chk(*(char *)tmp->data, " \t", F_WORD) == 0)
				return (ft_error(F_ERROR_AMB));
			tmp = tmp->nxt;
		}
		file_name = ft_c_str(data->file_name, NULL, -1, 1);
		if (file_name == NULL)
			return (ft_error(F_ERROR_MEM));
		if (ft_open(token, file_name, data->mode) != 0)
			return (1);
		free(file_name);
		redir = redir->nxt;
	}
	return (0);
}
