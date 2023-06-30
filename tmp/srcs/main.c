/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 02:29:34 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_lst	*ev;

	(void)argv;
	if (argc != 1)
		return (ft_error(F_ERROR_ARG, F_EXIT_STATUS_ARG));
	ev = mk_ev(env);
	if (ev == NULL)
		return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
	while (1)
		ft_parse(ev);
	return (0);
}
