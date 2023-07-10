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
	int	link_flag;

	link_flag = F_PIPE + F_OR + F_AND;
	if (**s == '|' && *flag == 0)
		*flag |= F_PIPE;
	else if (**s == '>' && *(*s + 1) == '>' && ((*flag) & ~link_flag) == 0)
		*flag |= F_APPEND;
	else if (**s == '>' && ((*flag) & ~link_flag) == 0)
		*flag |= F_OUTPUT;
	else if (**s == '<' && *(*s + 1) == '<' && ((*flag) & ~link_flag) == 0)
		*flag |= F_DEL;
	else if (**s == '<' && ((*flag) & ~link_flag) == 0)
		*flag |= F_INPUT;
	else
		return (ft_error(F_ERROR_SYNTAX, F_EXIT_STATUS_SYNTAX));
	(*s) += 1 + (((*flag) & F_APPEND) || ((*flag) & F_DEL));
	return (0);
}
