/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/01 05:37:49 by hyunghki         ###   ########.fr       */
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

# define F_QUOTE 1
# define F_DQUOTE 2
# define F_PIPE 4
# define F_INPUT 8
# define F_HEREDOC 16
# define F_OUTPUT 32
# define F_APPEND 64
# define F_DEL 128
# define F_NO_TRANS 256
# define F_NO_PARSE 512
# define F_DATA_CHAR 1
# define F_DATA_STRING 2
# define F_DATA_TOKEN 4
# define F_EXIT_STATUS_FLAG -1
# define F_EXIT_STATUS_MEM 1
# define F_EXIT_STATUS_SYNTAX 1
# define F_EXIT_STATUS_ARG 1
# define F_EXIT_STATUS_FILE 1
# define F_EXIT_STATUS_BUILTIN -2
# define F_ERROR_ARG "Too Many Arguments...\n"
# define F_ERROR_MEM "Memmory Is Not Enough...\n"
# define F_ERROR_SYNTAX "Unexpected Token...\n"
# define F_ERROR_FILE "No Such File or Directory...\n"
# define F_ERROR_ACCESS "Permission Denied...\n"

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

/**
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
int		ft_heredoc(t_lst *f, t_lst *ev);
**/
int		ft_lst_size(t_lst *lst);
t_lst	*mk_lst(void *data, int data_type, int info);
void	lst_push(t_lst **target, t_lst *to_push);
void	lst_delete(t_lst *to_del);
void	*ft_calloc(int size);
void	*ft_argv_free(char **argv);
void	ft_node_free(void *data, int data_type, int info);
void	*ft_lst_free(t_lst *lst);
void	*ft_argv_free(char **argv);
void	ft_putstr_fd(char *s, int fd);
int		ft_error(char *msg, int exit_status);
char	*ft_trim(char *s);
char	**mk_argv(t_lst *lst);
int		ft_str_find(char *s, char to_find);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlen(char *s);
char	*ft_substr(char *s, int n);
char	*ft_append(char *s1, char *s2, char middle);
char	*ft_itoa(int a, int cnt);
char	*ft_c_str(t_lst *lst);
t_lst	*mk_str_lst(char *s);
t_lst	*mk_ev(char **env);
t_lst	*ft_env_find(t_lst *ev, char *s);
void	ft_parse(t_lst *ev);
t_lst	*mk_token_lst(char *s, t_lst *ev);
int		ft_split(char *s, char *meta, int *flag, t_lst **target);
t_lst	*ft_split_space(char *s);
int		ft_pipe(t_lst *tv);
void	ft_dup2(int *fd, t_lst *tv);
int		ft_open(t_token *token);
void	ft_close(int *fd, t_lst *tv);
void	sigint_handler(int sig);
void	heredoc_signal_handler(int sig);
int		cal_flag(int ret_val);
void	ft_signal(void (*handler_1)(int), void (*handler_2)(int), int flag);

#endif
