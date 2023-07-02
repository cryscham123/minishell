/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 04:35:37 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_trim(char *s)
{
	int	i;

	while (*s && (*s == ' ' || *s == '\t'))
		s++;
	i = ft_strlen(s) - 1;
	while (i >= 0)
	{
		if (s[i] != ' ' && s[i] != '\t')
			break ;
		s[i] = '\0';
		i--;
	}
	return (s);
}

char	*ft_itoa(int a, int cnt)
{
	char	*target;

	if (a < 10)
	{
		target = ft_calloc(cnt + 1);
		target[0] = "0123456789"[a % 10];
		return (target);
	}
	target = ft_itoa(a / 10, cnt + 1);
	target[ft_strlen(target)] = "0123456789"[a % 10];
	return (target);
}

char	*ft_c_str(t_lst *lst)
{
	char	*target;
	int		i;

	target = ft_calloc(ft_lst_size(lst) + 1);
	if (target == NULL)
		return (NULL);
	i = 0;
	while (lst != NULL)
	{
		if (lst->info != F_NO_PARSE)
			target[i++] = *(char *)lst->data;
		lst = lst->nxt;
	}
	return (target);
}

char	**mk_argv(t_lst *lst)
{
	char	**target;
	int		size;
	int		i;

	size = ft_lst_size(lst);
	target = ft_calloc(sizeof(char *) * (size + 1));
	if (target == NULL)
		return (NULL);
	i = 0;
	while (lst != NULL)
	{
		target[i] = ft_substr(lst->data, -1);
		if (target[i] == NULL)
			return (ft_argv_free(target));
		i++;
		lst = lst->nxt;
	}
	return (target);
}

t_lst	*mk_str_lst(char *s)
{
	t_lst	*target;
	t_lst	*to_push;
	char	*data;

	target = NULL;
	while (*s)
	{
		data = ft_substr(s, 1);
		if (data == NULL)
			return (ft_lst_free(target));
		to_push = mk_lst(data, F_DATA_CHAR, 0);
		if (to_push == NULL)
		{
			free(data);
			return (ft_lst_free(target));
		}
		lst_push(&target, to_push);
		s++;
	}
	return (target);
}
