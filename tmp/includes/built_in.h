/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:28:20 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

#include "minishell.h"

# define F_EV_NO_VALUE -3
# define F_EV_KEY_ERROR -4

int		echo_option_chk(char *s);
int		ft_av_validate_dir(char **s);
int		ft_set_ev_pwd(char *key, t_lst *ev);
int		ft_av_validate_ev(char *s);
void	ft_unset_unit(char *s, t_lst *ev, int flag);
int		ft_av_env(char *av, t_lst *ev);
int		ft_av_exit(char *s);

#endif
