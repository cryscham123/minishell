/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/26 14:28:31 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <termios.h>

int	g_status;

typedef struct s_lst
{
	void			*data;
	int				data_type;
	int				info;
	struct s_lst	*nxt;
	struct s_lst	*prev;
}	t_lst;

typedef struct s_token
{
	t_lst	*argv;
	t_lst	*redir;
	int		fd[2];
}	t_token;

int		ft_echo(char **av);
int		ft_cd(char **av, t_lst *ev);
int		ft_pwd(void);
int		ft_export(char **av, t_lst *ev);
int		ft_unset(char **av, t_lst *ev);
int		ft_exit(char **av);
int		ft_env(char **av, t_lst *ev);
int		ft_exe(t_lst *tv, t_lst *ev);
int		ft_extern_cmd(char **av, char **env, t_lst *ev, int is_forked);
int		ft_built_in_cmd(char **argv, t_lst *ev);
void	ft_parse(t_lst *ev);
void	sigint_handler(int sig);
void	heredoc_signal_handler(int sig);
int		cal_flag(int ret_val);
void	ft_signal(void (*handler_1)(int), void (*handler_2)(int), int flag);
int		ft_lst_size(t_lst *lst);
t_lst	*mk_lst(void *data, int data_type, int info);
void	lst_push(t_lst **target, t_lst *to_push);
void	lst_delete(t_lst *to_del);
void	*ft_argv_free(char **argv);
char	**mk_argv(t_lst *lst);
int		ft_error(char *msg, int exit_status);
void	ft_putstr_fd(char *s, int fd);
int		ft_str_find(char *s, char to_find);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlen(char *s);
char	*ft_substr(char *s, int n);
char	*ft_append(char *s1, char *s2, char middle);
char	*ft_itoa(int a);
char	*ft_c_str(t_lst *lst);
void	*ft_calloc(int size);
t_lst	*mk_ev(char **env);
t_lst	*ft_env_find(t_lst *ev, char *s);

#endif
