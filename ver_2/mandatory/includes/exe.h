/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 02:30:19 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 08:17:15 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_H
# define EXE_H

# include "minishell.h"

# define F_STATUS_NO_BUILTIN -3
# define F_EXIT_STATUS_ACCESS 126
# define F_EXIT_STATUS_EXE 127
# define F_ERROR_NOT_FILE "Not A File...\n"
# define F_ERROR_EXE "Command Not Found...\n"

void	ft_exe_extern(char *path, char **av, char **env);
int		ft_wait_pid(int n);

#endif
