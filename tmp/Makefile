# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/07 06:51:22 by hyunghki          #+#    #+#              #
#    Updated: 2023/07/01 04:15:59 by hyunghki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
LDFLAGS= -L$(HOME)/.brew/opt/readline/lib
CFLAGS= -Wall -Werror -Wextra
DEBUG_FLAGS= -g3 -fsanitize=address
LIBS= -lreadline
INCLUDE= -Iincludes \
		 -I$(HOME)/.brew/opt/readline/include
SRC_DIR= ./srcs/
SRCS= $(SRC_DIR)main.c \
	  $(SRC_DIR)utils/env.c \
	  $(SRC_DIR)utils/heap_manage.c \
	  $(SRC_DIR)utils/lst.c \
	  $(SRC_DIR)utils/lst2.c \
	  $(SRC_DIR)utils/print.c \
	  $(SRC_DIR)utils/str.c \
	  $(SRC_DIR)utils/str2.c \
	  $(SRC_DIR)signal/interface.c \
	  $(SRC_DIR)parse/interface.c \
	  $(SRC_DIR)parse/split.c \
	  $(SRC_DIR)parse/token.c \
	  $(SRC_DIR)parse/expansion.c \
	  $(SRC_DIR)file_utils/interface.c \
	  $(SRC_DIR)file_utils/heredoc.c
	  #$(SRC_DIR)exe/interface.c \
	  #$(SRC_DIR)exe/utils.c \
	  #$(SRC_DIR)built_in/interface.c \
	  #$(SRC_DIR)built_in/interface2.c \
	  #$(SRC_DIR)built_in/utils.c \
	  #$(SRC_DIR)built_in/utils2.c
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
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS) $(DEBUG_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE) $(DEBUG_FLAGS)
