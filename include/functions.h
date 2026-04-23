/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Header file for functions and for includes
*/

#ifndef FUNCTIONS_H_
    #define FUNCTIONS_H_

    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <fcntl.h>
    #include "jobs.h"

    #define SEMICOLON ";"
    #define GLOBBINGS_CHARS "*?[]"
    #define GLOBBINGS_TMP_FILE ".globbings.tmp"

typedef struct builtins_s {
    char *name;
    char **(*ptr)(char **, char **, int *);
} builtins_t;

// builtins.c
char **print_env(char **arg, char **env, int *last_return);
void exit_program(char **commands_array, char **copy_env, int last_return,
    jobs_t **jobs);
char **execute_setenv(char **arg, char **copy_env, int *last_return);
char **execute_unsetenv(char **arg, char **copy_env, int *last_return);
// cd.c
char **execute_cd(char **arg, char **copy_env, int *last_return);
// exec.c
char *get_command_path(char *command, char **copy_env);
void handle_signal_error(int status, int *last_return, const char **env);
void execute_child(char **arg, char *path, char **copy_env);
void execute_command(char *command, char **copy_env, int *last_return,
    jobs_t **jobs);
// env_utils.c
char *build_env_variable(char *name, char *value);
int get_env_var_index(char **copy_env, char *name);
int is_name_valid(char *name, const char **env);
char **add_to_env(char **copy_env, char *new_var);
void detele_env_var(char **copy_env, int index);
// parsing.c
char **parse_command(char *command, char ***array, int *last_return,
    jobs_t **jobs);
// main.c
char **transform_to_string_array(char *str, char *separator);
void free_array(char **arg);
void print_exit(void);
char **process_line(char *line, char **copy_env, int *last_return,
    jobs_t **jobs);
// pipe.c
void handle_pipe(char *line, char **copy_env, int *last_return);
int pipe_syntax_error(char *line);
// redirections.c
int apply_redirection(char *command, const char **env);
// bonus.c
void display_custom_prompt(char **copy_env);
// tools.c
char *cut_ending_char(char *buffer, char c);
bool str_isnum(const char *str, int *val);
// subshell.c
int check_subshell(char *command, char **copy_env,
    int *last_return, jobs_t **jobs);
char **split_semicolon(char *line);

static const builtins_t builtins_functions[] = {
    {"cd", execute_cd},
    {"env", print_env},
    {"setenv", execute_setenv},
    {"unsetenv", execute_unsetenv},
    {NULL, NULL}
};

#endif
