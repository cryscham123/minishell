/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoklee <seoklee@student.42.kr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:59:30 by seoklee           #+#    #+#             */
/*   Updated: 2023/06/18 11:15:24 by seoklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	builtin_env(t_lst *ev)
{
	t_lst	value;

	while (ev != NULL)
	{
		value = ev->data->value;
		prinrf("%s=", ev->env->key);
		while (value != NULL)
		{
			write(1, ev->data->value->data, 1);
			value = value->nxt;
		}
		write(1, "\n", 1);
		ev = ev->nxt;
	}
	g_status = 0;
}