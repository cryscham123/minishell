/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 11:56:01 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:26:31 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_node_free(void *data, int data_info)
{
	if (data == NULL)
		return (NULL);
	if (data_info == F_DATA_STRING)
		return (ft_lst_free(data, NULL, F_DATA_CHAR, NULL));
	if (data_info == F_DATA_TOKEN)
	{
		ft_close(((t_token *)data)->fd, NULL);
		ft_lst_free(((t_token *)data)->argv, NULL, F_DATA_STRING, NULL);
		ft_lst_free(((t_token *)data)->redirection, NULL, F_DATA_FILE, NULL);
	}
	else if (data_info == F_DATA_FILE)
	{
		if (((t_file *)data)->mode == F_HEREDOC)
			ft_unlink(((t_file *)data)->file_name);
		ft_lst_free(((t_file *)data)->file_name, NULL, F_DATA_CHAR, NULL);
	}
	else if (data_info == F_DATA_HASH)
	{
		free(((t_hash *)data)->key);
		ft_lst_free(((t_hash *)data)->value, NULL, F_DATA_CHAR, NULL);
	}
	free(data);
	return (NULL);
}

void	*ft_lst_free(t_lst *lst, char **argv, int data_info, const char *msg)
{
	t_lst	*tmp;
	int		i;

	if (msg != NULL)
		ft_error(msg);
	i = 0;
	while (argv != NULL && argv[i] != NULL)
		free(argv[i++]);
	free(argv);
	while (lst != NULL)
	{
		ft_node_free(lst->data, data_info);
		tmp = lst;
		lst = lst->nxt;
		free(tmp);
	}		
	return (NULL);
}

t_lst	*mk_lst(void *data, int is_argv)
{
	t_lst	*target;

	target = ft_calloc(sizeof(t_lst));
	if (target == NULL)
	{
		if (is_argv)
			return (ft_lst_free(data, NULL, F_DATA_STRING, NULL));
		return (NULL);
	}
	target->data = data;
	return (target);
}

t_lst	*ft_lst_back(t_lst *lst)
{
	t_lst	*prev;

	prev = NULL;
	while (lst != NULL)
	{
		prev = lst;
		lst = lst->nxt;
	}
	return (prev);
}

int	lst_push(t_lst **lst, t_lst *data)
{
	t_lst	*tmp;

	if (data == NULL)
		return (ft_error(F_ERROR_MEM));
	tmp = *lst;
	if (tmp == NULL)
	{
		*lst = data;
		return (0);
	}
	while (tmp->nxt != NULL)
		tmp = tmp->nxt;
	tmp->nxt = data;
	return (0);
}
