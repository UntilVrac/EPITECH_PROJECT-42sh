/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Builtin functions
*/

#include "functions.h"
#include "lang.h"

char **print_env(char **arg, char **copy_env, int *last_return, void **structs)
{
    (void)arg;
    (void)structs;
    for (size_t i = 0; copy_env[i] != NULL; i++) {
        printf("%s\n", copy_env[i]);
    }
    *last_return = 0;
    return copy_env;
}

void exit_program(char **commands_array, char **copy_env, int last_return,
    void **structs)
{
    jobs_t **jobs = (jobs_t **)structs[0];
    history_t **history = (history_t **)structs[1];
    jobs_t *tmp = *jobs;
    size_t last = jobs_struct_len(tmp);

    if (tmp && tmp[0].state != NULL_STATE && tmp[last].state != EXITED) {
        tmp[last].state = EXITED;
        fprintf(stderr, "There are suspended jobs.\n");
        return;
    }
    free_history_struct(history);
    free_array(commands_array);
    free_jobs_struct(*jobs);
    free_array(copy_env);
    print_exit();
    exit(last_return);
}

static int fail_setenv(char **arg, int *last_return, const char **env)
{
    if (arg[1] && arg[2] && arg[3]) {
        print_error("setenv", TOO_M_ARGS, env);
        *last_return = 1;
        return -1;
    }
    if (is_name_valid(arg[1], env) == -1) {
        *last_return = 1;
        return -1;
    }
    return 0;
}

char **execute_setenv(char **arg, char **copy_env, int *last_return,
    void **structs)
{
    char *new_var = NULL;
    int index = 0;

    if (!arg[1])
        return print_env(arg, copy_env, last_return, structs);
    if (fail_setenv(arg, last_return, (const char **)(copy_env)) == -1)
        return copy_env;
    *last_return = 0;
    new_var = build_env_variable(arg[1], arg[2]);
    index = get_env_var_index((const char **)(copy_env), arg[1]);
    if (index != -1) {
        free(copy_env[index]);
        copy_env[index] = new_var;
        return copy_env;
    }
    return add_to_env(copy_env, new_var);
}

char **execute_unsetenv(char **arg, char **copy_env, int *last_return,
    void **structs)
{
    int index = 0;

    (void)structs;
    if (!arg[1]) {
        print_error("unsetenv", TOO_F_ARGS, (const char **)(copy_env));
        *last_return = 1;
        return copy_env;
    }
    *last_return = 0;
    for (int i = 1; arg[i] != NULL; i++) {
        index = get_env_var_index((const char **)(copy_env), arg[i]);
        if (index != -1) {
            detele_env_var(copy_env, index);
        }
    }
    return copy_env;
}
