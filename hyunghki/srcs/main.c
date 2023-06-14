/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/14 18:23:47 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_str(t_lst *val)
{
	while (val != NULL)
	{
		printf("%s", val->data);
		val = val->nxt;
	}
	printf("\n");
}

static void	ft_test(t_lst *tv, t_lst *ev)
{
	t_token	*data;

	(void)ev;
	while (tv != NULL)
	{
		printf("\n============ test =============\n");
		data = tv->data;
		printf("arguments:\n");
		while (data->argv != NULL)
		{
			print_str(data->argv->data);
			data->argv = data->argv->nxt;
		}
		printf("\nredirection:\n");
		while (data->redirection != NULL)
		{
			t_file *f = data->redirection->data;
			switch (f->mode)
			{
				case f_input:
					printf("input: ");
					break;
				case f_output:
					printf("output: ");
					break;
				case f_heredoc:
					printf("heredoc: ");
					break;
				case f_appand:
					printf("appand: ");
					break;
			}
			print_str(f->file_name);
			data->redirection = data->redirection->nxt;
		}
		printf("\n");
		tv = tv->nxt;
	}
}

static void	*ft_parse(t_lst *ev)
{
	char		*line;
	t_lst		*tv;

	tv = NULL;
	line = readline("$ ");
	if (line != NULL && *line)
	{
		add_history(line);
		tv = mk_token_lst(line, ev);
		if (tv == NULL)
		{
			free(line);
			return (NULL);
		}
		ft_test(tv, ev);
		ft_lst_free(tv, f_data_token, NULL);
	}
	free(line);
	return (NULL);
}

int	main(int argc, char **argv, char **env)
{
	t_lst		*ev;

	(void)argv;
	if (argc != 1)
		return (ft_error(f_error_arg));
	// signal handle
	ev = NULL;
	while (*env)
	{
		if (lst_push(&ev, mk_hash_lst(*env)) != 0)
		{
			ft_lst_free(ev, f_data_hash, NULL);
			return (0);
		}
		env++;
	}
	ft_test(NULL, ev);
	while (1)
		ft_parse(ev);
	return (0);
}
