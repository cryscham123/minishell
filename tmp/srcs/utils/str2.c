/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 01:06:52 by hyunghki         ###   ########.fr       */
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
		if (target == NULL)
			return (NULL);
		target[0] = "0123456789"[a % 10];
		return (target);
	}
	target = ft_itoa(a / 10, cnt + 1);
	if (target == NULL)
		return (NULL);
	target[ft_strlen(target)] = "0123456789"[a % 10];
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

t_lst	*mk_str_node(char *s, int info)
{
	t_lst	*ret;
	char	*tmp;

	tmp = ft_substr(s, -1);
	if (tmp == NULL)
		return (NULL);
	ret = mk_lst(tmp, F_DATA_CHAR, info);
	if (ret == NULL)
	{
		free(tmp);
		return (NULL);
	}
	return (ret);
}
