/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 09:42:12 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/11 09:42:15 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_flag_chk(char **s, int *flag)
{
	int	link_flag;

	link_flag = F_PIPE + F_OR + F_AND;
	if (**s == '|' && *(*s + 1) == '|' && *flag == 0)
		*flag |= F_OR;
	else if (**s == '|' && *flag == 0)
		*flag |= F_PIPE;
	else if (**s == '&' && *(*s + 1) == '&' && *flag == 0)
		*flag |= F_PIPE;
	else if (**s == '&')
		return (0);
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
	(*s) += 1 + (**s == *(*s + 1));
	return (0);
}
