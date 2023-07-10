/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 02:13:14 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_flag_chk(char **s, int *flag)
{
	if (*flag != 0)
	{
		*flag = 0;
		return (ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX));
	}
	if (**s == '|')
		*flag = F_PIPE;
	if (**s == '>')
	{
		if (*(*s + 1) == '>')
			*flag = F_APPEND;
		else
			*flag = F_OUTPUT;
	}
	if (**s == '<')
	{
		if (*(*s + 1) == '<')
			*flag = F_DEL;
		else
			*flag = F_INPUT;
	}
	(*s) += 1 + (*flag == F_APPEND || *flag == F_DEL);
	return (0);
}
