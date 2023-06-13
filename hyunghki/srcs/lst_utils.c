/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 11:56:01 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/13 13:44:56 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_lst_free(t_lst *lst, int data_info)
{
	t_lst	*tmp;

	while (lst != NULL)
	{
		if (data_info == 1)
		{
			ft_lst_free(((t_token *)lst->data)->argv, 0);
			ft_lst_free(((t_token *)lst->data)->redirection, 2);
		}
		else if (data_info == 2)
			free(((t_file *)lst->data)->file_name);
		free(lst->data);
		tmp = lst;
		lst = lst->nxt;
		free(tmp);
	}		
	return (NULL);
}

t_lst	*mk_lst(void *data, int data_type, int file_type)
{
	t_lst	*target;
	t_file	*f;

	target = ft_calloc(sizeof(t_lst));
	if (target == NULL)
	{
		free(data);
		return (NULL);
	}
	if (data_type == 2)
	{
		f = ft_calloc(sizeof(t_file));
		if (f == NULL)
		{
			free(data);
			free(target);
			return (NULL);
		}
		f->file_name = data;
		f->mode = file_type;
		target->data = f;
	}
	else
		target->data = data;
	return (target);
}

int	lst_push(t_lst **lst, t_lst *data)
{
	t_lst	*tmp;

	if (data == NULL)
		return (1);
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
