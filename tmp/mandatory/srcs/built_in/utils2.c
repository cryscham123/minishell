/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 06:16:44 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static void	ft_ev_exit_help(int is_forked)
{
	if (!is_forked)
		ft_putstr_fd("exit\n", 1);
	exit(ft_error(F_ERROR_EXIT, F_EXIT_STATUS_EXIT));
}

int	ft_av_exit(char *data, int is_forked)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	i = 0;
	sign = -(data[i] == '-') + (data[i] != '-');
	i += (data[i] == '+' || data[i] == '-');
	while (data[i])
	{
		if (!(data[i] >= '0' && data[i] <= '9') \
			|| (res == 922337203685477580 && data[i] > ('7' + (sign == -1))))
			ft_ev_exit_help(is_forked);
		res = res * 10 + data[i] - '0';
		i++;
	}
	if (((data[0] == '-' || data[0] == '+') && i == 1) || i == 0)
		ft_ev_exit_help(is_forked);
	return (res * sign);
}
