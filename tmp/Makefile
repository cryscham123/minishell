# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/07 06:51:22 by hyunghki          #+#    #+#              #
#    Updated: 2023/07/03 03:13:11 by hyunghki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
LDFLAGS= -L$(HOME)/.brew/opt/readline/lib
CFLAGS= -Wall -Werror -Wextra
DEBUG_FLAGS= -g3 -fsanitize=address
LIBS= -lreadline
INCLUDE= -Imandatory/includes \
		 -Ibonus/includes \
		 -I$(HOME)/.brew/opt/readline/include
SRC_DIR= ./mandatory/srcs/
BSRC_DIR= ./bonus/srcs/
SRCS= $(SRC_DIR)main.c \
	  $(SRC_DIR)utils/env.c \
	  $(SRC_DIR)utils/heap_manage.c \
	  $(SRC_DIR)utils/lst.c \
	  $(SRC_DIR)utils/print.c \
	  $(SRC_DIR)utils/str.c \
	  $(SRC_DIR)utils/str2.c \
	  $(SRC_DIR)signal/interface.c \
	  $(SRC_DIR)parse/interface.c \
	  $(SRC_DIR)parse/split.c \
	  $(SRC_DIR)parse/token.c \
	  $(SRC_DIR)parse/expansion.c \
	  $(SRC_DIR)parse/expansion2.c \
	  $(SRC_DIR)file_utils/interface.c \
	  $(SRC_DIR)file_utils/heredoc.c \
	  $(SRC_DIR)exe/interface.c \
	  $(SRC_DIR)exe/utils.c \
	  $(SRC_DIR)built_in/interface.c \
	  $(SRC_DIR)built_in/interface2.c \
	  $(SRC_DIR)built_in/utils.c \
	  $(SRC_DIR)built_in/utils2.c
BSRCS= $(BSRC_DIR)main_bonus.c \
	   $(BSRC_DIR)utils/env_bonus.c \
	   $(BSRC_DIR)utils/heap_manage_bonus.c \
	   $(BSRC_DIR)utils/lst_bonus.c \
	   $(BSRC_DIR)utils/print_bonus.c \
	   $(BSRC_DIR)utils/str_bonus.c \
	   $(BSRC_DIR)utils/str2_bonus.c \
	   $(BSRC_DIR)signal/interface_bonus.c \
	   $(BSRC_DIR)parse/interface_bonus.c \
	   $(BSRC_DIR)parse/split_bonus.c \
	   $(BSRC_DIR)parse/token_bonus.c \
	   $(BSRC_DIR)parse/expansion_bonus.c \
	   $(BSRC_DIR)parse/expansion2_bonus.c \
	   $(BSRC_DIR)file_utils/interface_bonus.c \
	   $(BSRC_DIR)file_utils/heredoc_bonus.c \
	   $(BSRC_DIR)exe/interface_bonus.c \
	   $(BSRC_DIR)exe/utils_bonus.c \
	   $(BSRC_DIR)built_in/interface_bonus.c \
	   $(BSRC_DIR)built_in/interface2_bonus.c \
	   $(BSRC_DIR)built_in/utils_bonus.c \
	   $(BSRC_DIR)built_in/utils2_bonus.c
OBJS= $(SRCS:.c=.o)
BOBJS= $(BSRCS:.c=.o)

ifdef WITH_BONUS
	OBJ_FILE = $(BOBJS)
else
	OBJ_FILE = $(OBJS)
endif

all: $(NAME)

.PHONY: re clean fclean bonus

re:
	make fclean
	make all

clean:
	rm -f $(OBJS)

fclean:
	make clean
	rm -f $(NAME)

bonus:
	@make WITH_BONUS=1 all

$(NAME): $(OBJ_FILE)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)
