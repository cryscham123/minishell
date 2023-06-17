/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/17 15:59:45 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
// int rl_on_new_line(void)
// void rl_redisplay(void)
// void rl_replace_line(const char *, int)
# include <readline/history.h>
// void rl_clear_history(void)
# include <stdlib.h>
// pid_t wait(int *status)
// pid_t wait3(int *statloc, int options, struct rusage *rusage)
// pid_t wait4(pid_t pid, int *statloc, int options, struct rusage *rusage)
// pid_t waitpid(pid_t pid, int *status, int options);
# include <unistd.h>
// int access(const char *path_name, int mode)
// ssize_t read(int fd, void *buf, size_t nbyte)
// pid_t fork(void)
// char *getcwd(char *buf, size_t size)
// int chdir(const char *path)
// int stat(const char *path, struct stat *buf)
// int fstat(int fd, struct stat *buf)
// int lstat(const char *path, struct stat *buf)
// int unlink(const char *pathname)
// int execve(const char *filename, char *const argv[],char *const envp[])
// int dup(int oldfd)
// int dup2(int oldfd, int newfd)
// int pipe(int pipefd[2])
// int isatty(int fd)
// char *ttyname(int fd)
// int ttyslot(void)
// int tcgetattr(int fd, struct termios *termios_p)
// int tcsetattr(int fd, int optional_actions, const struct termios *termios_p)
# include <fcntl.h>
# include <signal.h>
// int kill(pid_t pid, int sig)
// sighandler_t signal(int signum, sighandler_t handler)
// int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
// int sigemptyset(sigset_t *set)
// int sigaddset(sigset_t *set, int signum)
# include <dirent.h>
// DIR *opendir(const char *name)
// struct dirent *readdir(DIR *dirp)
// int closedir(DIR *dirp)

# define f_error_mem "Memmory Allocation Failed..."
# define f_error_syntax "Unexpected Token..."
# define f_error_amb "Ambiguos Arguments..."
# define f_error_arg "Too Many Arguments..."
# define f_error_file "Failed To Access File Or Directory..."
# define f_get 0
# define f_reset 1
# define f_chk 2
# define f_word 3
# define f_data_char 0
# define f_data_string 1
# define f_data_file 2
# define f_data_hash 3
# define f_data_token 4
# define f_quote 1
# define f_dequote 2
# define f_pipe 4
# define f_input 8
# define f_heredoc 16
# define f_output 32
# define f_append 64

typedef struct s_lst
{
	void			*data;
	int				size;
	struct s_lst	*nxt;
}	t_lst;

typedef struct s_hash
{
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

t_lst	*ft_itoa(int a);
char	*ft_substr(char *src, int n);
void	*ft_calloc(int size);
int		ft_strcmp(char *s1, char *s2);
int		ft_word_chk(char c, char *meta, int mode);
int		ft_split(void *target, char *s, char *meta, int flag);
int		ft_error(const char *msg);
int		ft_expansion(t_lst *lst, int is_redir, t_lst *ev);
int		ft_trans_ev(t_lst **cur, t_lst **prev, t_lst **lst, t_lst *ev);
int		ft_resplit(t_token *token, t_lst *lst, int n);
int		ft_redirection(t_token *token, t_lst *redir, t_lst *ev);
t_lst	*mk_str_lst(char *s);
t_lst	*mk_hash_lst(char *s);
t_lst	*mk_token_lst(char *line, t_lst *ev);
t_lst	*mk_file_lst(char *s, int dir_type);
t_lst	*ft_hash_find(t_lst *hash, char *to_find);
int		dup_str_lst(t_lst **lst, t_lst *to_dup);
char	*ft_c_str(t_lst *str, int n, int del_quote);
t_lst	*mk_lst(void *data, int is_argv);
void	*ft_node_free(void *data, int data_info);
void	*ft_lst_free(t_lst *lst, int data_info, const char *msg);
int		lst_push(t_lst **lst, t_lst *data);
t_lst	*ft_lst_back(t_lst *lst);
int		ft_heredoc(t_file *f, int token_num, t_lst *ev);
void	ft_close(int *fd);

#endif
