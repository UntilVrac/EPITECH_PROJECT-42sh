/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Input parsing and command execution flow
*/

#include <string.h>
#include "functions.h"
#include "lang.h"

static int count(char *str, char *separator)
{
    int count = 0;
    char *copy = NULL;
    char *token = NULL;

    if (!str)
        return 0;
    copy = strdup((const char *)(str));
    token = strtok(copy, separator);
    while (token) {
        count++;
        token = strtok(NULL, separator);
    }
    free(copy);
    return count;
}

char **transform_to_string_array(char *str, char *separator)
{
    char **arg = malloc(sizeof(char *) * (count(str, separator) + 1));
    int index = 0;
    char *token = NULL;

    if (!arg) {
        perror("malloc error transform");
        exit(84);
    }
    token = strtok(str, separator);
    while (token) {
        arg[index] = strdup((const char *)(token));
        index++;
        token = strtok(NULL, separator);
    }
    arg[index] = NULL;
    return arg;
}

static char **execute_builtin(char **arg, char **copy_env, int *last_return)
{
    char **result_env = NULL;

    if (strcmp((const char *)(arg[0]), "cd") == 0)
        result_env = execute_cd(arg, copy_env, last_return);
    if (strcmp((const char *)(arg[0]), "setenv") == 0 && arg[1] != NULL)
        result_env = execute_setenv(arg, copy_env, last_return);
    if (strcmp((const char *)(arg[0]), "unsetenv") == 0)
        result_env = execute_unsetenv(arg, copy_env, last_return);
    if (strcmp((const char *)(arg[0]), "env") == 0 ||
        (strcmp((const char *)(arg[0]), "setenv") == 0 && arg[1] == NULL)) {
        print_env(copy_env, last_return);
        result_env = copy_env;
    }
    if (result_env) {
        free_array(arg);
        return result_env;
    }
    return NULL;
}

static char **check_builtins(char *command, char **copy_env,
    int *last_return, char **commands_array)
{
    char *command_copy = strdup((const char *)(command));
    char **arg = transform_to_string_array(command_copy, " \t");
    char **result_env = NULL;

    free(command_copy);
    if (!arg || !arg[0]) {
        free_array(arg);
        return copy_env;
    }
    if (strcmp((const char *)(arg[0]), "exit") == 0) {
        free_array(commands_array);
        exit_program(arg, copy_env, *last_return);
    }
    result_env = execute_builtin(arg, copy_env, last_return);
    if (result_env)
        return result_env;
    free_array(arg);
    execute_command(command, copy_env, last_return);
    return copy_env;
}

char **parse_command(char *command, char **copy_env,
    int *last_return, char **commands_array)
{
    if (strchr((const char *)(command), '|') != NULL) {
        if (pipe_syntax_error(command) == -1) {
            print_error(NULL, NULL_CMD, (const char **)(copy_env));
            *last_return = 1;
            return copy_env;
        }
        handle_pipe(command, copy_env, last_return);
        return copy_env;
    }
    return check_builtins(command, copy_env, last_return, commands_array);
}
