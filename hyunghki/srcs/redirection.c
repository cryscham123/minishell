/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:09:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/18 18:49:23 by hyunghki         ###   ########.fr       */
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

void	ft_close(int *fd)
{
	if (fd[0] != 0)
		close(fd[0]);
	if (fd[1] != 1)
		close(fd[1]);
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
