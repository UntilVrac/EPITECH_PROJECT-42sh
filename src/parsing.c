/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Input parsing and command execution flow
*/

#include "functions.h"
#include "lang.h"

static void replace_backslash_with_separator(char **arg, size_t *x,
    char *separator)
{
    char *tmp = NULL;

    arg[*x] = realloc(arg[*x], sizeof(char *) * (strlen(arg[*x]) +
            strlen(separator)));
    if (!arg[*x])
        exit(84);
    tmp = my_str_concat(arg[*x], separator);
    free(arg[*x]);
    arg[*x] = strdup(tmp);
    if (!arg[*x])
        exit(84);
    free(tmp);
}

static char *check_backslash(char **arg, size_t *x, char *separator)
{
    char *tmp = NULL;
    size_t nb_loop = 0;

    while (arg[*x][strlen(arg[*x]) - 1] == '\\') {
        if (strlen(separator) > 1)
            replace_backslash_with_separator(arg, x, separator);
        else
            arg[*x - nb_loop][strlen(arg[*x - nb_loop]) - 1] = separator[0];
        if (!arg[*x + 1])
            break;
        else {
            tmp = my_str_concat(arg[*x - nb_loop], arg[*x + 1]);
            free(arg[*x - nb_loop]);
            arg[*x - nb_loop] = strdup(tmp);
            free(tmp);
        }
        *x += 1;
        nb_loop++;
    }
    return arg[*x - nb_loop];
}

static size_t count_without_backslash(char **arg, size_t count_nb)
{
    for (size_t x = 0; arg[x] != NULL; x++)
        if (arg[x][strlen(arg[x]) - 1] == '\\' && arg[x + 1] != NULL)
            count_nb--;
    return count_nb;
}

static char **check_inhibitors(char **arg, char *separator, size_t count_nb)
{
    char **new_arg = NULL;
    size_t current = 0;

    count_nb = count_without_backslash(arg, count_nb);
    new_arg = malloc(sizeof(char *) * (count_nb + 1));
    if (!new_arg)
        exit(84);
    for (size_t x = 0; arg[x] != NULL; x++) {
        if (arg[x][strlen(arg[x]) - 1] == '\\')
            new_arg[current] = strdup(check_backslash(arg, &x, separator));
        else
            new_arg[current] = strdup(arg[x]);
        if (!new_arg[current])
            exit(84);
        current++;
    }
    new_arg[current] = NULL;
    free_array(arg);
    return new_arg;
}

static size_t count(char *str, char *separator)
{
    size_t count = 0;
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
    size_t count_nb = count(str, separator);
    char **arg = malloc(sizeof(char *) * (count_nb + 1));
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
    arg = check_inhibitors(arg, separator, count_nb);
    return arg;
}

static char **execute_builtin(char **arg, char **copy_env, int *last_return,
    jobs_t **jobs)
{
    char **result_env = NULL;

    for (size_t i = 0; jobs_builtins[i].name; i++)
        if (!strcmp(jobs_builtins[i].name, arg[0])) {
            jobs_builtins[i].ptr(arg, (const char **)(copy_env), jobs,
                last_return);
            return copy_env;
        }
    if (arg[0][0] == '%') {
        job_control_synonym(arg, last_return, (const char **)(copy_env), jobs);
        return copy_env;
    }
    for (size_t i = 0; builtins_functions[i].name; i++)
        if (!strcmp(arg[0], builtins_functions[i].name)) {
            result_env = builtins_functions[i].ptr(arg, copy_env, last_return);
            return result_env;
        }
    return NULL;
}

static char **exec_all(char *command, char ***array,
    int *last_return, jobs_t **jobs)
{
    char **copy_env = array[0];
    char **arg = array[1];
    char **result_env = NULL;

    result_env = execute_builtin(arg, copy_env, last_return, jobs);
    free_array(arg);
    if (result_env)
        return result_env;
    execute_command(command, copy_env, last_return, jobs);
    return copy_env;
}

static char **check_builtins(char *command, char ***array,
    int *last_return, jobs_t **jobs)
{
    char *command_copy = strdup((const char *)(command));
    char **arg = transform_to_string_array(command_copy, " \t");
    char **copy_env = array[0];
    char **commands_array = array[1];
    jobs_t *tmp = *jobs;

    free(command_copy);
    if (!arg || !arg[0]) {
        free_array(arg);
        return copy_env;
    }
    if (strcmp((const char *)(arg[0]), "exit") == 0) {
        free_array(arg);
        exit_program(commands_array, copy_env, *last_return, jobs);
        return copy_env;
    }
    if (tmp[jobs_struct_len(tmp)].state == EXITED)
        tmp[jobs_struct_len(tmp)].state = NULL_STATE;
    return exec_all(command, (char **[]) {copy_env, arg}, last_return, jobs);
}

char **parse_command(char *command, char ***array,
    int *last_return, jobs_t **jobs)
{
    char **copy_env = array[0];
    char **commands_array = array[1];

    (void)commands_array;
    if (check_subshell(command, copy_env, last_return, jobs))
        return copy_env;
    if (strchr((const char *)(command), '|') != NULL) {
        if (pipe_syntax_error(command) == -1) {
            print_error(NULL, NULL_CMD, (const char **)(copy_env));
            *last_return = 1;
            return copy_env;
        }
        handle_pipe(command, copy_env, last_return);
        return copy_env;
    }
    return check_builtins(command, array, last_return, jobs);
}
