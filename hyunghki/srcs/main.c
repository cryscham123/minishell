/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/13 14:18:00 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_lst		*ev;

	(void)argv;
	if (argc != 1)
	{
		printf("Too Many Arguments.\n");
		return (0);
	}
	// signal handle
	ev = NULL;
	while (*env)
	{
		if (lst_push(&ev, mk_lst(*env, 0, 0)) != 0)
		{
			printf("Memmory Allocation Failed...\n");
			return (0);
		}
		env++;
	}
	while (1)
		ft_parse(ev);
	return (0);
}
