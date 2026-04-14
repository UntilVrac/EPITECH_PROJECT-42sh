/*
** EPITECH PROJECT, 2026
** MINISHELL 2
** File description:
** Builtin functions
*/

#include "../include/functions.h"

void print_env(char **copy_env, int *last_return)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        my_putstr(copy_env[i]);
        write(1, "\n", 1);
    }
    *last_return = 0;
}

void exit_program(char **arg, char **copy_env, int last_return)
{
    free_array(arg);
    free_array(copy_env);
    exit(last_return);
}

static int fail_setenv(char **arg, int *last_return)
{
    const char *error = "setenv: Too many arguments.\n";

    if (arg[1] && arg[2] && arg[3]) {
        write(2, error, my_strlen((char *)error));
        *last_return = 1;
        return -1;
    }
    if (is_name_valid(arg[1]) == -1) {
        *last_return = 1;
        return -1;
    }
    return 0;
}

char **execute_setenv(char **arg, char **copy_env, int *last_return)
{
    char *new_var = NULL;
    int index = 0;

    if (fail_setenv(arg, last_return) == -1)
        return copy_env;
    *last_return = 0;
    new_var = build_env_variable(arg[1], arg[2]);
    index = get_env_var_index(copy_env, arg[1]);
    if (index != -1) {
        free(copy_env[index]);
        copy_env[index] = new_var;
        return copy_env;
    }
    return add_to_env(copy_env, new_var);
}

char **execute_unsetenv(char **arg, char **copy_env, int *last_return)
{
    const char *error = "unsetenv: Too few arguments.\n";
    int index = 0;

    if (!arg[1]) {
        write(2, error, my_strlen((char *)error));
        *last_return = 1;
        return copy_env;
    }
    *last_return = 0;
    for (int i = 1; arg[i] != NULL; i++) {
        index = get_env_var_index(copy_env, arg[i]);
        if (index != -1) {
            detele_env_var(copy_env, index);
        }
    }
    return copy_env;
}
