/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/03 04:28:40 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	ft_av_exit(char *data)
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
			|| (res == 214748364 && data[i] > ('7' + (sign == -1))))
		{
			ft_putstr_fd("exit\n", 1);
			exit(ft_error(F_ERROR_EXIT, F_EXIT_STATUS_EXIT));
		}
		res = res * 10 + data[i] - '0';
		i++;
	}
	if (((data[0] == '-' || data[0] == '+') && i == 1) || i == 0)
	{
		ft_putstr_fd("exit\n", 1);
		exit(ft_error(F_ERROR_EXIT, F_EXIT_STATUS_EXIT));
	}
	return (res * sign);
}
