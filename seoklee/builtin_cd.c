/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoklee <seoklee@student.42.kr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 14:09:47 by seoklee           #+#    #+#             */
/*   Updated: 2023/06/18 01:11:48 by seoklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (i + j + 1));
	if (str == NULL)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = -1;
	while (s2[++j])
		str[i + j] = s2[j];
	str[i + j] = '\0';
	free(s1);
	return (str);
}

char	*get_env_value(t_lst *value)
{
	char	*str;

	if (!value || !value->data)
		return (NULL);
	str = ft_substr(value->data, 1);
	//if (str == NULL)
	while (value->nxt != NULL)
	{
		str = ft_strjoin(str, value->nxt->data); //첫 인자 str은 strjoin 함수 안에서 free
		//if (str == NULL)
		value = value->nxt;
	}
	return (str);
}

void	change_dir(char *path, t_lst *ev)
{
	char	 cwd[4096];
	t_lst	*value;

	getcwd(cwd, 4096);
	if (!chdir(path))
	{
		g_status = 0;
		value = ft_hash_find(ev, "OLDPWD");
		ft_lst_free(value, f_data_char, NULL);
		value = mk_str_lst(cwd);
		value = ft_hash_find(ev, "PWD");
		ft_lst_free(value, f_data_char, NULL);
		getcwd(cwd, 4096);
		value = mk_str_lst(cwd);
	}
	else
		ft_error("cd: fail");
}

void	builtin_cd(char **argv, t_lst *ev)
{
	char	*home_path;
	t_lst	*value;

	value = ft_hash_find(ev, "HOME");
	home_path = get_env_value(value);
	if (argv[1] == NULL && value == NULL)
		ft_error("HOME not set");
	else if (argv[1] == NULL)
		change_dir(home_path, ev);
	else
		change_dir(argv[1], ev);
}
