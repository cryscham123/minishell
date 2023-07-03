/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 08:22:57 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe.h"

static void	chk_access_and_exe(char *path, char **argv, char **env)
{
	struct stat	buf;

	if (access(path, F_OK) != 0)
		return ;
	stat(path, &buf);
	if (!S_ISREG(buf.st_mode))
		exit(ft_error(F_ERROR_NOT_FILE, F_EXIT_STATUS_ACCESS));
	if (access(path, X_OK) != 0)
		exit(ft_error(F_ERROR_ACCESS, F_EXIT_STATUS_ACCESS));
	execve(path, argv, env);
}

void	ft_exe_extern(char *path, char **av, char **env)
{
	char	*target_path;
	int		i;

	if (*av[0] == '\0')
		exit(ft_error(F_ERROR_EXE, F_EXIT_STATUS_EXE));
	ft_signal(SIG_DFL, SIG_DFL, 1);
	chk_access_and_exe(av[0], av, env);
	while (path != NULL)
	{
		i = ft_str_find(path, ':');
		if (i != -1)
			path[i] = '\0';
		target_path = ft_append(path, av[0], '/');
		if (target_path == NULL)
			exit(F_EXIT_STATUS_MEM);
		chk_access_and_exe(target_path, av, env);
		free(target_path);
		if (i == -1)
			break ;
		else
			path += (i + 1);
	}
	exit(ft_error(F_ERROR_EXE, F_EXIT_STATUS_EXE));
}

int	ft_wait_pid(int n)
{
	pid_t	pid;
	pid_t	pid_tmp;
	int		flag;
	int		ret;

	pid = 0;
	while (n--)
	{
		pid_tmp = waitpid(-1, &flag, 0);
		if (pid_tmp > pid)
		{
			pid = pid_tmp;
			ret = flag;
		}
	}
	if (flag == 2)
		ft_putstr_fd("\n", 1);
	return (ret);
}
