##
## EPITECH PROJECT, 2026
## G2 - Shell Programming - 42sh
## File description:
## Makefile for building the minishell 2 project
##

CC := epiclang
CPPFLAGS = -iquote include/
CFLAGS = -Wall -Wextra -Werror

SRC = src/main.c									\
	src/my_lib.c									\
	src/builtins.c									\
	src/parsing.c									\
	src/env_utils.c									\
	src/exec.c										\
	src/cd.c										\
	src/my_lib2.c									\
	src/pipe.c										\
	src/redirections.c								\
	bonus/bonus.c									\
	src/print_error/file_tool_functions.c			\
	src/print_error/get_env_dir.c					\
	src/print_error/my_str_concat.c					\
	src/print_error/print_error.c					\
	src/print_error/search_into_str_functions.c		\
	src/print_error/str_tools_functions.c

OBJ = $(SRC:.c=.o)

all: mysh

mysh: $(OBJ)
	$(CC) -o 42sh $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f 42sh

re: fclean all

.PHONY: all clean fclean re
