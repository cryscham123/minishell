/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 23:15:43 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 13:28:38 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

t_lst	*ft_trans_status(char **s)
{
	t_lst	*ret;
	char	*num;

	num = ft_itoa(g_status, 1);
	ret = mk_str_lst(num);
	free(num);
	(*s)++;
	return (ret);
}

t_lst	*ft_trans_dollar(char **s, t_lst *ev, int flag)
{
	t_lst	*to_find;
	char	*target;
	char	tmp;
	int		i;

	if (flag == 0 && (*(*s + 1) == '\'' || *(*s + 1) == '\"'))
		return (NULL);
	(*s)++;
	i = 0;
	while ((*s)[i] && ft_str_find("&| \t><$\'\"=*", (*s)[i]) == -1)
		i++;
	tmp = (*s)[i];
	(*s)[i] = '\0';
	to_find = ft_env_find(ev, *s);
	(*s)[i] = tmp;
	*s += (i - 1);
	if (to_find == NULL)
		return (NULL);
	target = (char *)to_find->data + (to_find->info + 1);
	return (mk_str_lst(target));
}
