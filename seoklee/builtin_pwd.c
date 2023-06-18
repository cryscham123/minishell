/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoklee <seoklee@student.42.kr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 23:59:50 by seoklee           #+#    #+#             */
/*   Updated: 2023/06/18 00:04:36 by seoklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pwd(char **argv)
{
	char	cwd[4096];

	if (argv[1])
	{
		g_status = 1;
		ft_error("pwd: Too many arguments.");
		return ;
	}
	getcwd(cwd, 4096);
	printf("%s\n", cwd);
	g_status = 0;
}