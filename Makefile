##
## EPITECH PROJECT, 2026
## MINISHELL 2
## File description:
## Makefile for building the minishell 2 project
##

CC := epiclang
CFLAGS = -Wall -Wextra -Werror -I. -g3

SRC = src/main.c \
	src/my_lib.c \
	src/builtins.c \
	src/parsing.c \
	src/env_utils.c \
	src/exec.c \
	src/cd.c \
	src/my_lib2.c \
	src/pipe.c \
	src/redirections.c \
	bonus/bonus.c
OBJ = $(SRC:.c=.o)

all: mysh

mysh: $(OBJ)
	$(CC) -o mysh $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f mysh

re: fclean all

.PHONY: all clean fclean re
