/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:22:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*ft_split(char *s, char *meta, int data_type)
{
	int		i;
	int		meta_type;
	t_lst	*target;

	while (*s)
	{
		while (s[i])
		{
			meta_type = ft_chr_chk(s[i], meta);
			if (meta_type < 0)
				return (NULL);
			else if (meta_type == 0)
				break ;
			i++;
		}
		s += i;
		while (s[i] && ft_chr_chk(s[i], meta))
			i++;
		s += i;
	}
	return (target);
}
