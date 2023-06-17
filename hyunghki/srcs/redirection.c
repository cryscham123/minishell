/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:09:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/17 13:53:05 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_heredoc(int fd, char *del, t_lst *ev)
{
	int		i;
	char	*target;

	while (1)
	{
		target = readline("\033[34mheredoc>\033[0m ");
		if (ft_strcmp(del, target) == 0)
			break;
		
		free(target);
	}
	free(target);
	i = 1;
	cmp = *target;
	while (ft_word_chk(cmp[i], "| \t><$\'\"", f_chk) != 0)
		i++;
	return (0);
}

static t_lst *create_heredoc(char *del, int mode, int token_num, t_lst *ev)
{
	int		fd;
	t_lst	*file;
	char	*target;
	char	*tmp;

	file = ft_itoa(token_num);
	if (file == NULL)
		return (f_error_mem);
	tmp = ft_c_str(file);
	if (tmp == NULL)
		return (ft_lst_free(file, f_data_char, f_error_mem));
	fd = open(tmp, O_CREAT | RDWR, 0644);
	free(tmp);
	if (fd < 0)
		return (ft_lst_free(file, f_data_char, f_error_file));
	if (parse_heredoc(fd, del, ev) != 0)
	{
		close(fd);
		return (ft_lst_free(file, f_data_char, f_error_file));
	}
	close(fd);
	return (file);
}

int	ft_heredoc(t_file *f, int token_num, t_lst *ev)
{
	char	*del
	int		mode;
	int		fd;
	t_lst	*tmp;

	tmp = f->file_name;
	del = ft_c_str(tmp, -1, 0);
	if (del == NULL)
		return (1);
	mode = 0;
	while (tmp != NULL)
	{
		if (ft_word_chk(*(char *)tmp->data, " \t", f_chk) == 0)
			mode = 1;
		tmp = tmp->nxt;
	}
	tmp = create_heredoc(del, mode, token_num, ev);
	free(del);
	if (tmp == NULL)
		return (1);
	lst_free_lst(f->file_name, f_data_char, NULL);
	f->file_name = tmp;
	return (0);
}

static int	ft_open(t_token *token, char *s, int mode, t_lst *ev)
{
	int	fd;
	int	direction;

	(void)ev;
	if (mode == f_input || mode == heredoc)
		fd = open(s, O_RDONLY);
	else if (mode == f_output)
		fd = open(s, O_CREAT | O_RDWR, 0644);
	else
		fd = open(s, O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		free(s);
		return (ft_error(f_error_file));
	}
	direction = (mode == f_input || mode == f_heredoc);
	if (token->fd[direction] != direction)
		close(token->fd[direction]);
	token->fd[direction] = fd;
	return (0);
}

int	ft_redirection(t_token *token, t_lst *redir, t_lst *ev)
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
			if (ft_word_chk(*(char *)tmp->data, " \t", f_word) == 0)
				return (ft_error(f_error_amb));
			tmp = tmp->nxt;
		}
		file_name = ft_c_str(data->file_name, -1, 1);
		if (file_name == NULL)
			return (ft_error(f_error_mem));
		if (ft_open(token, file_name, data->mode, ev) != 0)
			return (1);
		free(file_name);
		redir = redir->nxt;
	}
	return (0);
}
