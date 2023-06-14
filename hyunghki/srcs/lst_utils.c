/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 11:56:01 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/14 18:24:50 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_node_free(void *data, int data_info)
{
	if (data == NULL)
		return (NULL);
	if (data_info == f_data_string)
		return (ft_lst_free(data, f_data_char, NULL));
	if (data_info == f_data_token)
	{
		ft_lst_free(((t_token *)data)->argv, f_data_string, NULL);
		ft_lst_free(((t_token *)data)->redirection, f_data_file, NULL);
	}
	else if (data_info == f_data_file)
		ft_lst_free(((t_file *)data)->file_name, f_data_char, NULL);
	else if (data_info == f_data_hash)
	{
		free(((t_hash *)data)->key);
		ft_lst_free(((t_hash *)data)->value, f_data_char, NULL);
	}
	free(data);
	return (NULL);
}

void	*ft_lst_free(t_lst *lst, int data_info, const char *msg)
{
	t_lst	*tmp;

	if (msg != NULL)
		ft_error(msg);
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
			return (ft_lst_free(data, f_data_string, NULL));
		return (NULL);
	}
	target->data = data;
	return (target);
}

int	lst_push(t_lst **lst, t_lst *data)
{
	t_lst	*tmp;

	if (data == NULL)
		return (ft_error(f_error_mem));
	tmp = *lst;
	if (tmp == NULL)
	{
		*lst = data;
		(*lst)->size++;
		return (0);
	}
	while (tmp->nxt != NULL)
		tmp = tmp->nxt;
	tmp->nxt = data;
	(*lst)->size++;
	return (0);
}
