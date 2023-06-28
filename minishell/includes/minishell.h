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
# include <termios.h>

# define F_ERROR_MEM "Memmory Allocation Failed..."
# define F_ERROR_SYNTAX "Unexpected Token..."
# define F_ERROR_ARG "Too Many Arguments..."
# define F_ERROR_FILE "Failed To Access File Or Directory..."
# define F_ERROR_EXPORT "Not A Valid Identifier..."
# define F_ERROR_EXIT "Not A Number..."
# define F_ERROR_EXE "Command Not Found..."
# define F_ERROR_ACCESS "Permission Denied..."
# define F_GET 0
# define F_RESET 1
# define F_CHK 2
# define F_WORD 3
# define F_DATA_CHAR 0
# define F_DATA_STRING 1
# define F_DATA_FILE 2
# define F_DATA_HASH 3
# define F_DATA_TOKEN 4
# define F_QUOTE 2
# define F_DQUOTE 4
# define F_PIPE 8
# define F_INPUT 16
# define F_HEREDOC 32
# define F_OUTPUT 64
# define F_APPEND 128
# define F_NO_TRANS 256
# define F_STATUS_NO_BUILTIN -1
# define F_ERROR_BUILTIN -2

typedef struct s_lst
{
	void			*data;
	int				size;
	struct s_lst	*nxt;
}	t_lst;

typedef struct s_hash
{
	char	**env;
	char	*key;
	t_lst	*value;
}	t_hash;

typedef struct s_file
{
	t_lst	*file_name;
	int		mode;
}	t_file;

typedef struct s_token
{
	t_lst	*argv;
	t_lst	*redirection;
	int		fd[2];
}	t_token;

char	*ft_itoa(int a);
char	*ft_substr(char *src, int n);
void	*ft_calloc(int size);
int		ft_strcmp(char *s1, char *s2);
int		ft_word_chk(char c, char *meta, int mode);
int		ft_split(void *target, char *s, char *meta, int flag);
int		ft_error(const char *msg);
int		ft_expansion(t_lst *lst, int is_redir, t_lst *ev);
int		ft_redirection(t_token *token, t_lst *redir, int is_single);
t_lst	*mk_str_lst(char *s);
t_lst	*mk_hash_lst(char *s);
t_lst	*mk_token_lst(char *line, t_lst *ev);
t_lst	*mk_file_lst(char *s, int dir_type);
t_lst	*ft_hash_find(t_lst *hash, char *to_find);
int		dup_str_lst(t_lst **lst, t_lst *to_dup);
int		ft_str_size(t_lst *str);
char	*ft_c_str(t_lst *str, t_lst *apd, char c, int n);
t_lst	*mk_lst(void *data, int is_argv);
void	*ft_node_free(void *data, int data_info);
void	*ft_lst_free(t_lst *lst, char **argv, int data_info, const char *msg);
int		lst_push(t_lst **lst, t_lst *data);
t_lst	*ft_lst_back(t_lst *lst);
int		ft_heredoc(t_file *f, t_lst *ev, char *file_name);
void	ft_unlink(t_lst *file_name);
void	ft_close(int *fd, t_lst *tv);
int		ft_pipe(t_lst *tv);
int		ft_built_in_cmd(t_lst *argv, t_lst *ev);
int		ft_export(t_lst *argv, t_lst *ev, int flag);
int		ft_unset(t_lst	*argv, t_lst *ev, int flag);
int		ft_env(t_lst *ev);
int		ft_echo(t_lst *argv);
int		ft_cd(t_lst *argv, t_lst *ev);
int		ft_pwd(void);
int		ft_chk_key(char *s, t_lst *ev, int cmd);
int		ft_exit(t_lst *argv);
int		ft_exit_code(t_lst *data);
int		ft_exe(t_lst *tv, t_lst *ev, int i);
void	ft_signal(void (*handler_1)(int), void (*handler_2)(int), int flag);
void	heredoc_signal_handler(int sig);
void	sigint_handler(int sig);
int		cal_flag(int ret_val);
char	*ft_hash_str(t_hash *data);
int		ft_set_ev_pwd(char *key, t_lst *ev, t_hash *pwd);
int		echo_option_chk(t_lst *str);
void	ft_exe_extern(t_lst *path, t_lst *av, char **argv, t_lst *ev);

#endif
