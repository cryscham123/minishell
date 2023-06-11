/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/11 15:46:12 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_token_v	tv;

	(void)argv;
	(void)env;
	if (argc != 1)
	{
		printf("argument error\n");
		return (0);
	}
	while (1)
	{
		tv.cur_token = NULL;
		tv.cur_token_len = 0;
		line = readline("$ ");
		if (line != NULL && *line)
		{
			add_history(line);
			if (ft_parse_data(&tv, line) != 0)
			{
				printf("error occured...\n");
				return (0);
			}
			//ft_exe(&tv, env);
		}
		free(line);
		//ft_free_all(line, tv->cur_token);
	}
	return (0);
}
