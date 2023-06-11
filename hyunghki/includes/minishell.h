/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:47 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/11 15:45:42 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// ioctl <- ?
# include <stdio.h>
// int printf(const char *format, ...)
// void perror(const char *s)
# include <readline/readline.h>
// char *readline(const char *prompt)
// int rl_on_new_line(void)
// void rl_redisplay(void)
// void rl_replace_line(const char *, int)
# include <readline/history.h>
// void add_history(char *line)
// void rl_clear_history(void)
# include <stdlib.h>
// void *malloc(size_t size)
// void free(void *ptr)
// void exit(int status)
// pid_t wait(int *status)
// pid_t wait3(int *statloc, int options, struct rusage *rusage)
// pid_t wait4(pid_t pid, int *statloc, int options, struct rusage *rusage)
// pid_t waitpid(pid_t pid, int *status, int options);
// char *getenv(const char *name);
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
// int open(const char *path_name, int o_flag)
// int close(int fd)
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
# include <string.h>
// char *strerror(int errnum)

# define f_input 1
# define f_output 2
# define f_heredoc 3
# define f_appand 4

typedef struct s_file
{
	char			*file_name;
	int				mode;
	struct s_file	*nxt_file;
}	t_file;

typedef struct s_token
{
	char			**argv;
	t_file			*redirection;
	int				arg_size;
	struct s_token	*nxt_token;
}	t_token;

typedef struct s_token_v
{
	t_token	*cur_token;
	int		cur_token_len;
}	t_token_v;

char	**ft_split(char **s, char *meta);
void	*ft_calloc(int size);
void	ft_strncpy(char *dst, char *src, int n);
int		ft_parse_data(t_token_v *tv, char *line);

#endif
