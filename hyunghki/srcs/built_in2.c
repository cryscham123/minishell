/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:39:04 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/18 16:35:15 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_chk_validate(char *s, int cmd)
{
	int	i;

	i = 0;
	while (s[i] && ft_word_chk(s[i], "=", f_chk) != 0)
	{
		if (!(i != 0 && s[i] >= '0' && s[i] <= '9') \
			&& !(s[i] >= 'a' && s[i] <= 'z') \
			&& !(s[i] >= 'A' && s[i] <= 'Z') && s[i] != '_')
			return (-1);
		i++;
	}
	if (i == 0 || (cmd == 1 && s[i] == '='))
	{
		ft_error(f_error_export);
		return (-1);
	}
	if (cmd == 0 && s[i] != '=')
		return (-1);
	return (i);
}

static int	ft_chk_key(char *s, t_lst *ev, int cmd)
{
	int		i;
	char	tmp;
	t_lst	*prev;

	if (*s >= '0' && *s <= '9')
		return (ft_error(f_error_export));
	i = ft_chk_validate(s, cmd);
	if (i < 0)
		return (1);
	tmp = s[i];
	s[i] = '\0';
	while (ev != NULL)
	{
		if (ft_strcmp(((t_hash *)ev->data)->key, s) == 0)
		{
			prev->nxt = ev->nxt;
			ft_node_free(ev, f_data_hash);
			break ;
		}
		prev = ev;
		ev = ev->nxt;
	}
	s[i] = tmp;
	return (0);
}

int	ft_export(t_lst *argv, t_lst *ev, int flag)
{
	char	*tmp;

	if (argv == NULL)
		return (ft_env(ev));
	while (argv != NULL)
	{
		tmp = ft_c_str(argv->data, NULL, -1, 1);
		if (tmp == NULL)
			return (ft_error(f_error_mem));
		flag = ft_chk_key(tmp, ev, 0);
		if (flag == 0 && lst_push(&ev, mk_hash_lst(tmp)) != 0)
		{
			free(tmp);
			return (ft_error(f_error_mem));
		}
		free(tmp);
		argv = argv->nxt;
	}
	return (flag);
}

int	ft_unset(t_lst	*argv, t_lst *ev, int flag)
{
	char	*tmp;

	while (argv != NULL)
	{
		tmp = ft_c_str(argv->data, NULL, -1, 1);
		if (tmp == NULL)
			return (ft_error(f_error_mem));
		flag = ft_chk_key(tmp, ev, 1);
		free(tmp);
		argv = argv->nxt;
	}
	return (flag);
}

int	ft_env(t_lst *ev)
{
	t_lst	*val;

	while (ev != NULL)
	{
		printf("%s=", ((t_hash *)ev->data)->key);
		val = ((t_hash *)ev->data)->value;
		while (val != NULL)
		{
			printf("%c", *(char *)val->data);
			val = val->nxt;
		}
		printf("\n");
		ev = ev->nxt;
	}
	return (0);
}
