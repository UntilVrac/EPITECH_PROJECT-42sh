/*
** EPITECH PROJECT, 2026
** all_executions.c
** File description:
** manage all the execution of the given commands
*/

#include "functions.h"

static char **execute_other_builtins(char **arg, char **copy_env,
    int *last_return, history_t **history)
{
    char **result_env = NULL;

    for (size_t i = 0; builtins_functions[i].name; i++)
        if (!strcmp(arg[0], builtins_functions[i].name)) {
            result_env = builtins_functions[i].ptr(arg, copy_env, last_return);
            return result_env;
        }
    if (strcmp(arg[0], "history") == 0) {
        history_management((const char **)arg, history);
        return copy_env;
    }
    return NULL;
}

char **execute_builtin(char **arg, char **copy_env, int *last_return,
    void **structs)
{
    jobs_t **jobs = (jobs_t **)structs[0];
    history_t **history = (history_t **)structs[1];

    check_background_jobs(jobs);
    for (size_t i = 0; jobs_builtins[i].name; i++)
        if (!strcmp(jobs_builtins[i].name, arg[0])) {
            *last_return = 0;
            jobs_builtins[i].ptr(arg, (const char **)(copy_env), jobs,
                last_return);
            return copy_env;
        }
    if (arg[0][0] == '%') {
        job_control_synonym(arg, last_return, (const char **)(copy_env), jobs);
        return copy_env;
    }
    return execute_other_builtins(arg, copy_env, last_return, history);
}

char **exec_all(char *command, char ***array,
    int *last_return, void **structs)
{
    jobs_t **jobs = (jobs_t **)structs[0];
    char **copy_env = array[0];
    char **arg = array[1];
    char **result_env = NULL;

    result_env = execute_builtin(arg, copy_env, last_return, structs);
    free_array(arg);
    if (result_env)
        return result_env;
    execute_command(command, (const char **)(copy_env), last_return, jobs);
    return copy_env;
}
