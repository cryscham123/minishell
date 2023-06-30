/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:28:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_argv_free(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		free(argv[i++]);
	free(argv);
	return (NULL);
}

void	ft_node_free(void *data, int data_type, int info)
{
	if (data == NULL)
		return ;
	if (info == F_HEREDOC)
		unlink((char *)data);
	if (data_type == F_DATA_STRING)
		ft_lst_free(data);
	else if (data_type == F_DATA_TOKEN)
	{
		ft_close(((t_token *)data)->fd, NULL);
		ft_lst_free(((t_token *)data)->argv);
		ft_lst_free(((t_token *)data)->redirection);
	}
	free(data);
}

void	*ft_lst_free(t_lst *lst)
{
	t_lst	*tmp;

	while (lst != NULL)
	{
		ft_node_free(lst->data, lst->data_type, lst->info);
		if (lst->prev != NULL)
			lst->prev->nxt = NULL;
		if (lst->nxt != NULL)
			lst->nxt->prev = NULL;
		tmp = lst;
		lst = lst->nxt;
		free(tmp);
	}
	return (NULL);
}

void	*ft_calloc(int size)
{
	int				i;
	unsigned char	*target;

	target = malloc(size);
	if (target == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		target[i] = 0;
		i++;
	}
	return (target);
}
