# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/07 06:51:22 by hyunghki          #+#    #+#              #
#    Updated: 2023/06/18 17:47:42 by hyunghki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
CFLAGS= -Wall -Werror -Wextra
LIBS= -lreadline
INCLUDE= -Iincludes
SRC_DIR= ./srcs/
SRCS= $(SRC_DIR)main.c \
	  $(SRC_DIR)utils.c \
	  $(SRC_DIR)split.c \
	  $(SRC_DIR)expansion.c \
	  $(SRC_DIR)lst_utils.c \
	  $(SRC_DIR)struct_produce.c \
	  $(SRC_DIR)redirection.c \
	  $(SRC_DIR)heredoc.c \
	  $(SRC_DIR)struct_utils.c \
	  $(SRC_DIR)exe.c \
	  $(SRC_DIR)built_in.c \
	  $(SRC_DIR)built_in2.c \
	  $(SRC_DIR)export_utils.c
OBJS= $(SRCS:.c=.o)

all: $(NAME)

.PHONY: re clean fclean

re:
	make fclean
	make all

clean:
	rm -f $(OBJS)

fclean:
	make clean
	rm -f $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)
