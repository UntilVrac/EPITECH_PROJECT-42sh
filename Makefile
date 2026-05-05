##
## EPITECH PROJECT, 2026
## G2 - Shell Programming - 42sh
## File description:
## Makefile
##

CC	:=	epiclang

CPPFLAGS	+=	-iquote ./include/
CFLAGS		+=	-Wall -Wextra
# LDFLAGS		+=	-L lib
# LDLIBS		+=	-l my


TESTS_FOLDER	=	tests/



GCOVR_FLAGS		=	--gcov-executable "llvm-cov-20 gcov" --exclude $(TESTS_FOLDER)
GCOVR_BRANCH	=	--branch
VALGRIND_FLAGS	=	--leak-check=full --show-leak-kinds=all



SRC			=	bonus/print_error/str_tools_functions.c				\
				bonus/print_error/search_into_str_functions.c		\
				bonus/print_error/my_str_concat.c					\
				bonus/print_error/file_tool_functions.c				\
				bonus/print_error/get_env_dir.c						\
				bonus/print_error/print_error.c						\
				bonus/bonus.c										\
				src/free_tools.c									\
				src/tools.c											\
				src/builtins/builtins.c								\
				src/builtins/cd.c									\
				src/builtins/env_utils.c							\
				src/exec.c											\
				src/jobs/get_jobs.c									\
				src/jobs/jobs_control.c								\
				src/jobs/manage_jobs.c								\
				src/jobs/jobs_struct.c								\
				src/jobs/background.c								\
				src/parsing.c										\
				src/pipe.c											\
				src/redirections.c									\
				src/subshell.c										\
				src/backticks.c										\
				src/globbings/globbings.c							\
				src/inhibitors.c									\
				src/quote_inhibitor.c								\
				src/line.c
				src/alias/alias_utils.c								\
				src/alias/alias_builtins.c							\
				src/line_edition/multiline.c						\
				src/line_edition/binding.c

MAIN_SRC	=	src/main.c # main file

TESTS_SRC	=	# unit tests src files


OBJ			=	$(SRC:.c=.o)
MAIN_OBJ	=	$(MAIN_SRC:.c=.o)
TESTS_OBJ	=	$(TESTS_SRC:.c=.o)


DELIVERY_NAME	=	42sh
TESTS_NAME		=	unit_tests



all: $(OBJ) $(MAIN_OBJ)
# 	$(MAKE) -C lib/my/
	$(CC) -o $(DELIVERY_NAME) $(OBJ) $(MAIN_OBJ) $(LDFLAGS) $(LDLIBS)



clean:
# 	$(MAKE) clean -C lib/my/
	$(RM) -f *~ \#*\#
	$(RM) -f $(OBJ) $(MAIN_OBJ) $(TESTS_OBJ)

fclean: clean
# 	$(MAKE) fclean -C lib/my/
	$(RM) -f $(DELIVERY_NAME) $(TESTS_NAME)
	$(RM) -f *.gc*

re: fclean all



runmain: all
	./$(DELIVERY_NAME)



tests_clean: fclean

unit_tests: LDFLAGS += --coverage -lcriterion -lgcov
unit_tests: fclean $(OBJ) $(TESTS_OBJ)
	$(CC) -o $(TESTS_NAME) $(OBJ) $(TESTS_OBJ) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

tests_run: unit_tests
	./$(TESTS_NAME)

cover: unit_tests
	gcovr $(GCOVR_FLAGS)
	gcovr $(GCOVR_FLAGS) $(GCOVR_BRANCH)

tests_all: tests_run cover



# glibmy:
# 	$(MAKE) gdebug -C lib/my/

# vlibmy:
# 	$(MAKE) vdebug -C lib/my/

# dlibmy:
# 	$(MAKE) debug -C lib/my/


gdebug: CFLAGS += -g
# gdebug: clean glibmy all
gdebug: clean all
	gdb ./$(DELIVERY_NAME)

vdebug: CPPFLAGS += -g3
# vdebug: clean vlibmy all
vdebug: clean all
	valgrind $(VALGRIND_FLAGS) ./$(DELIVERY_NAME)

debug: CFLAGS += -g
debug: CPPFLAGS += -g3
# debug: clean dlibmy re
debug: clean all



.PHONY: all clean fclean re runmain tests_clean unit_tests tests_run cover tests_all glibmy vlibmy dlibmy gdebug vdebug debug
