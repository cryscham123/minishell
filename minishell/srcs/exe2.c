/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:28:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	chk_access_and_exe(char *path, char **argv, char **env)
{
	if (access(path, F_OK) != 0)
		return ;
	execve(path, argv, env);
	exit(ft_error(F_ERROR_ACCESS));
}

void	ft_exe_extern(t_lst *path, t_lst *av, char **argv, t_lst *ev)
{
	char	*path_char;
	int		i;
	t_lst	*cur;

	ft_signal(SIG_DFL, SIG_DFL, 1);
	
	chk_access_and_exe(argv[0], argv, ((t_hash *)ev->data)->env);
	while (path != NULL)
	{
		cur = path;
		i = 0;
		while (cur != NULL && *(char *)cur->data != ':')
		{
			i++;
			cur = cur->nxt;
		}
		path_char = ft_c_str(path, av, '/', i);
		if (path_char == NULL)
			exit(ft_error(F_ERROR_MEM));
		chk_access_and_exe(path_char, argv, ((t_hash *)ev->data)->env);
		free(path_char);
		if (cur == NULL)
			break ;
		path = cur->nxt;
	}
	exit(ft_error(F_ERROR_EXE));
}
