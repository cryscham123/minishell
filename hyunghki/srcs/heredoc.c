/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 14:17:05 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/20 18:39:43 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_parse_heredoc_env(int fd, char **target, t_lst *ev)
{
	t_lst	*ev_val;
	char	tmp;
	int		i;

	i = 0;
	(*target)++;
	while ((*target)[i])
	{
		if (ft_word_chk((*target)[i], "| \t><$\'\"", F_CHK) == 0)
			break ;
		i++;
	}
	tmp = (*target)[i];
	(*target)[i] = '\0';
	ev_val = ft_hash_find(ev, (*target));
	(*target)[i] = tmp;
	while (ev_val != NULL)
	{
		write(fd, ev_val->data, 1);
		ev_val = ev_val->nxt;
	}
	(*target) += i;
}

static void	parse_heredoc(int fd, char *del, int mode, t_lst *ev)
{
	char	*tmp;
	char	*target;

	while (1)
	{
		ft_signal(heredoc_signal_handler, SIG_IGN);
		target = readline("\033[34mheredoc>\033[0m ");
		if (target == NULL)
			printf("\033[A\033[34mheredoc>\033[0m ");
		if (target == NULL || ft_strcmp(del, target) == 0)
			break ;
		tmp = target;
		while (*tmp)
		{
			if (*tmp == '$' && mode == 1)
				ft_parse_heredoc_env(fd, &tmp, ev);
			else
				write(fd, tmp++, 1);
		}
		write(fd, "\n", 1);
		free(target);
	}
	free(target);
}

static t_lst	*create_heredoc(char *del, int mode, int token_num, t_lst *ev)
{
	int		fd;
	t_lst	*file;
	char	*tmp;

	file = ft_itoa(token_num);
	if (file == NULL)
		return (ft_lst_free(file, F_DATA_CHAR, F_ERROR_MEM));
	tmp = ft_c_str(file, NULL, -1, 0);
	if (tmp == NULL)
		return (ft_lst_free(file, F_DATA_CHAR, F_ERROR_MEM));
	fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0644);
	free(tmp);
	if (fd < 0)
		return (ft_lst_free(file, F_DATA_CHAR, F_ERROR_FILE));
	parse_heredoc(fd, del, mode, ev);
	ft_signal(sigint_handler, sigquit_handler);
	close(fd);
	return (file);
}

int	ft_heredoc(t_file *f, int token_num, t_lst *ev)
{
	char	*del;
	int		mode;
	t_lst	*tmp;

	tmp = f->file_name;
	del = ft_c_str(tmp, NULL, -1, 1);
	if (del == NULL)
		return (ft_error(F_ERROR_MEM));
	mode = 1;
	while (tmp != NULL)
	{
		if (ft_word_chk(*(char *)tmp->data, "\'\"", F_CHK) == 0)
			mode = 0;
		tmp = tmp->nxt;
	}
	tmp = create_heredoc(del, mode, token_num, ev);
	free(del);
	if (tmp == NULL)
		return (1);
	ft_lst_free(f->file_name, F_DATA_CHAR, NULL);
	f->file_name = tmp;
	return (0);
}
