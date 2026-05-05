/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Input parsing and command execution flow
*/

#include "functions.h"
#include "lang.h"
#include "alias.h"

/*static void replace_backslash_with_separator(char **arg, size_t *x,
    char *separator)
{
    char *tmp = NULL;
//
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
//
static char *check_backslash(char **arg, size_t *x, char *separator)
{
    char *tmp = NULL;
    size_t nb_loop = 0;
//
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
//
static size_t count_without_backslash(char **arg, size_t count_nb)
{
    for (size_t x = 0; arg[x] != NULL; x++)
        if (arg[x][strlen(arg[x]) - 1] == '\\' && arg[x + 1] != NULL)
            count_nb--;
    return count_nb;
}
//
static char **check_inhibitors(char **arg, char *separator, size_t count_nb)
{
    char **new_arg = NULL;
    size_t current = 0;
//
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
//
static size_t count(char *str, char *separator)
{
    size_t count = 0;
    char *copy = NULL;
    char *token = NULL;
//
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
//
char **transform_to_string_array(char *str, char *separator)
{
    size_t count_nb = count(str, separator);
    char **arg = malloc(sizeof(char *) * (count_nb + 1));
    int index = 0;
    char *token = NULL;
//
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
*/
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

char **execute_builtin(char **arg, char **copy_env, int *last_return,
    jobs_t **jobs)
{
    char **result_env = NULL;

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
    execute_command(command, (const char **)(copy_env), last_return, jobs);
    return copy_env;
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

static char **check_builtins(char *command, void *array[],
    int *last_return, jobs_t **jobs)
{
    char **arg = transform_to_string_array((const char *)(command), " \t");
    char **copy_env = (char **)array[0];
    char **res = NULL;

    arg = apply_globbings_on_args(arg, NULL);
    if (!arg || !arg[0]) {
        free_array(arg);
        return copy_env;
    }
    res = handle_alias_logic(arg, array, last_return);
    if (res)
        return res;
    if (strcmp((const char *)(arg[0]), "exit") == 0) {
        free_array(arg);
        exit_program((char **)array[2], copy_env, *last_return, jobs);
        return copy_env;
    }
    update_job_state(jobs);
    return exec_all(command, (char **[]) {copy_env, arg}, last_return, jobs);
}

char **parse_command(char *command, void *array[],
    int *last_return, jobs_t **jobs)
{
    char **copy_env = (char **)array[0];
    alias_t **alias_list = (alias_t **)array[1];
    char **commands_array = (char **)array[2];

    (void)commands_array;
    if (check_subshell(command, copy_env, last_return, jobs))
        return copy_env;
    if (strchr((const char *)(command), '|') != NULL) {
        if (pipe_syntax_error(command) == -1) {
            print_error(NULL, NULL_CMD, (const char **)(copy_env));
            *last_return = 1;
            return copy_env;
        }
        handle_pipe(command, copy_env, last_return, alias_list);
        return copy_env;
    }
    return check_builtins(command, array, last_return, jobs);
}
