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
    #include "alias.h"

    #define SEMICOLON ";"
    #define GLOBBINGS_CHARS "*?[]"
    #define GLOBBINGS_TMP_FILE ".globbings.tmp"
    #define GLOB_FLAGS (GLOB_NOCHECK)
    #define BACKSLASH '\\'
    #define SIMPLE_QUOTE '\''
    #define DOUBLE_QUOTE '\"'

typedef int signal_t;

static const signal_t ignored_signals[5] = {
    SIGINT, SIGTSTP, SIGTTIN, SIGTTOU, 0};

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
void execute_command(const char *command, const char **copy_env,
    int *last_return, jobs_t **jobs);
// env_utils.c
char *build_env_variable(char *name, char *value);
int get_env_var_index(const char **copy_env, char *name);
int is_name_valid(char *name, const char **env);
char **add_to_env(char **copy_env, char *new_var);
void detele_env_var(char **copy_env, int index);
// parsing.c
char **execute_builtin(char **arg, char **copy_env, int *last_return,
    jobs_t **jobs);
char **parse_command(char *command, void *array[],
    int *last_return, jobs_t **jobs);
// main.c
char **transform_to_string_array(const char *str, const char *separator);
void my_replace_in_str(char *str, char c_init, char c_new);
void free_array(char **arg);
void print_exit(void);
char **process_line(void *data[], char **copy_env, int *last_return,
    jobs_t **jobs);
// pipe.c
void handle_pipe(char *line, char **copy_env,
    int *last_return, alias_t **alias_list);
int pipe_syntax_error(char *line);
// redirections.c
int apply_redirection(const char *command, const char **env);
// bonus.c
void display_custom_prompt(char **copy_env);
// tools.c
char *cut_ending_char(char *buffer, char c);
bool str_isnum(const char *str, int *val);
char **split_semicolon(char *line);
// subshell.c
void update_depth(char character, int *depth);
int check_subshell(char *command, char **copy_env,
    int *last_return, jobs_t **jobs);
// backticks.c
char *handle_backticks(char *line, int *last_return, jobs_t **jobs,
    char **env);
// globbings.c
int get_paths_with_globbings(char **str, const char **env);
char *get_path_with_home_dir(const char *str_init, const char **env);
char **apply_globbings_on_args(char **args, const char **env);
// free_tools.c
void nfree(size_t n, ...);
void free_array(char **arg);
void nfree_array(size_t n, ...);
//inhibitors.c
char **manage_quotes(char **new_arg, const char *separator);
//quote_inhibitor.c
bool char_cmp_quote(bool containing_quote, char character, char quote);
char **create_part(char **tmp, const char **new_arg, size_t **indexes,
    const char **utils);
void remove_backslash(const char **new_arg, char **tmp, char quote);

static const builtins_t builtins_functions[] = {
    {"cd", execute_cd},
    {"env", print_env},
    {"setenv", execute_setenv},
    {"unsetenv", execute_unsetenv},
    {NULL, NULL}
};

#endif /* FUNCTIONS_H_ */
