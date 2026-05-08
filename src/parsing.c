/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Input parsing and command execution flow
*/

#include <string.h>
#include "functions.h"
#include "lang.h"
#include "alias.h"

void my_replace_in_str(char *str, char c_init, char c_new)
{
    size_t size = 0;

    if (str == NULL)
        return;
    size = strlen((const char *)(str));
    for (size_t i = 0; i < size; ++i) {
        if (str[i] == c_init)
            str[i] = c_new;
        if (str[i] == '\\')
            i += 1;
    }
}

void rm_backslash(char *str)
{
    size_t j = 0;

    for (size_t i = 0; str[i] != '\0'; ++i) {
        if (str[i] != '\\') {
            str[j] = str[i];
            j += 1;
        }
    }
    str[j] = '\0';
}

static char **remove_empty_strings(char **arr_init, const char *separator)
{
    char **arr_new = calloc(my_word_array_len((const char **)(arr_init)) + 1,
        sizeof(char *));
    size_t index = 0;

    if (arr_new == NULL) {
        free_array((char **)(arr_init));
        return NULL;
    }
    for (size_t i = 0; arr_init[i] != NULL; ++i) {
        if (strlen(arr_init[i]) > 0) {
            arr_new[index] = arr_init[i];
            index += 1;
        } else
            free(arr_init[i]);
    }
    free(arr_init);
    arr_new = manage_quotes(arr_new, separator);
    return arr_new;
}

char **transform_to_string_array(const char *str, const char *separator)
{
    char *tmp = NULL;
    char **arr = NULL;

    if (str == NULL || separator == NULL)
        return NULL;
    tmp = strdup(str);
    if (tmp == NULL)
        return NULL;
    for (size_t i = 0; separator[i] != '\0'; ++i)
        my_replace_in_str(tmp, separator[i], '\n');
    rm_backslash(tmp);
    arr = my_split_str((const char *)(tmp), '\n');
    free(tmp);
    if (arr == NULL)
        return NULL;
    return remove_empty_strings(arr, separator);
}

static void update_job_state(jobs_t **jobs)
{
    jobs_t *tmp = *jobs;
    size_t len = jobs_struct_len(tmp);

    if (tmp[len].state == EXITED)
        tmp[len].state = NULL_STATE;
}

static char **handle_alias_logic(char **arg, void *array[], int *last_return)
{
    char **res = check_alias_expansion(arg, array, last_return);

    if (res)
        return res;
    if (check_alias_builtin(arg, (alias_t **)array[1])) {
        free_array(arg);
        return (char **)array[0];
    }
    return NULL;
}

static char **exec_exit_builtin(char **arg, int *last_return, void *array[])
{
    char **copy_env = (char **)array[0];
    alias_t **alias_list = (alias_t **)array[1];
    char **commands_array = (char **)array[2];
    jobs_t **jobs = (jobs_t **)array[3];
    jobs_t *tmp = *jobs;
    size_t last = jobs_struct_len(tmp);

    free_array(arg);
    if (tmp && tmp[0].state != NULL_STATE && tmp[last].state != EXITED) {
        tmp[last].state = EXITED;
        fprintf(stderr, "There are suspended jobs.\n");
        return copy_env;
    }
    free_history_struct((history_t **)array[4]);
    free_jobs_struct(*jobs);
    free_alias_list(*alias_list);
    nfree_array(2, commands_array, copy_env);
    print_exit();
    exit(*last_return);
    return copy_env;
}

char **check_builtins(char *command, void *array[],
    int *last_return, jobs_t **jobs)
{
    char **arg = transform_to_string_array((const char *)(command), " \t");
    char **copy_env = (char **)array[0];
    char **res = NULL;

    arg = apply_globbings_on_args(arg, (const char **)(array[0]));
    if (!arg || !arg[0]) {
        free_array(arg);
        return copy_env;
    }
    res = handle_alias_logic(arg, array, last_return);
    if (res)
        return res;
    if (strcmp((const char *)(arg[0]), EXIT_CMD) == 0)
        return exec_exit_builtin(arg, last_return, array);
    update_job_state(jobs);
    return exec_all(command, arg, last_return, array);
}

static int has_logic_ops(const char *cmd)
{
    int single_quote = 0;
    int double_quote = 0;
    int depth = 0;

    for (int i = 0; cmd[i] != '\0'; i++) {
        measure_depth(cmd[i], &depth, &single_quote, &double_quote);
        if (single_quote || double_quote || depth)
            continue;
        if (cmd[i] == '&' && cmd[i + 1] == '&')
            return 1;
        if (cmd[i] == '|' && cmd[i + 1] == '|')
            return 1;
    }
    return 0;
}

// copy_env
// &alias_list
// commands_array
// jobs
// history
char **parse_command(char *command, void *array[],
    int *last_return, jobs_t **jobs)
{
    char **copy_env = (char **)array[0];
    history_t **history = (history_t **)array[4];

    if (check_subshell(command, copy_env, last_return,
            (void *[]){jobs, history}))
        return copy_env;
    if (has_logic_ops(command))
        return handle_ops(command, array, last_return, jobs);
    if (strchr((const char *)(command), '|') != NULL) {
        if (pipe_syntax_error(command) == -1) {
            print_error(NULL, NULL_CMD, (const char **)(copy_env));
            *last_return = 1;
            return copy_env;
        }
        handle_pipe(command, last_return, array);
        return copy_env;
    }
    return check_builtins(command, array, last_return, jobs);
}
