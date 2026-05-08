/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Handling alias logic
*/

#include <string.h>
#include "functions.h"
#include "lang.h"
#include "jobs.h"
#include "alias.h"

static void display_aliases(alias_t *list)
{
    for (alias_t *tmp = list; tmp != NULL; tmp = tmp->next_alias)
        printf("%s\t%s\n", tmp->name, tmp->command);
}

static void trim_quotes(char *str)
{
    int len = strlen(str);

    if (len >= 2) {
        if ((str[0] == '\'' && str[len - 1] == '\'') ||
            (str[0] == '\"' && str[len - 1] == '\"')) {
            memmove(str, str + 1, len - 2);
            str[len - 2] = '\0';
        }
    }
}

static char *get_full_command(char **arg)
{
    char *full_command = NULL;
    int len = 0;

    for (int i = 2; arg[i] != NULL; i++)
        len += strlen(arg[i]) + 1;
    full_command = malloc(sizeof(char) * (len + 1));
    if (!full_command)
        return NULL;
    full_command[0] = '\0';
    for (int i = 2; arg[i] != NULL; i++) {
        strcat(full_command, arg[i]);
        if (arg[i + 1] != NULL)
            strcat(full_command, " ");
    }
    return full_command;
}

static void exec_alias(alias_t **list, char **arg)
{
    char *command = NULL;

    if (!arg[1]) {
        display_aliases(*list);
        return;
    }
    if (!arg[2]) {
        command = get_alias(*list, arg[1]);
        if (command)
            printf("%s\n", command);
        return;
    }
    command = get_full_command(arg);
    if (command && command[0] != '\0') {
        trim_quotes(command);
        add_alias(list, arg[1], command);
        free(command);
    }
}

static void exec_unalias(alias_t **list, char **arg)
{
    const char *error = "unalias: Too few arguments.\n";

    if (!arg || !arg[1]) {
        write(2, error, strlen(error));
        return;
    }
    for (int i = 1; arg[i] != NULL; i++)
        delete_alias(list, arg[i]);
}

int check_alias_builtin(char **arg, alias_t **alias_list)
{
    if (strcmp(arg[0], "alias") == 0) {
        exec_alias(alias_list, arg);
        return 1;
    }
    if (strcmp(arg[0], "unalias") == 0) {
        exec_unalias(alias_list, arg);
        return 1;
    }
    return 0;
}

char *build_alias_command(const char *alias_val, const char **old_arg)
{
    int size_alias = 0;
    int size_old = my_word_array_len(old_arg);
    char **arr = NULL;
    char *str = NULL;
    const char **alias =
        (const char **)(transform_to_string_array(alias_val, " \t"));

    if (!alias_val || !old_arg)
        return NULL;
    if (!alias) {
        free_array((char **)(alias));
        return NULL;
    }
    size_alias = my_word_array_len(alias);
    arr = calloc(size_alias + size_old + 1, sizeof(char *));
    if (arr && cpy_string_arr(old_arg, arr, false, old_arg) &&
        cpy_string_arr(&(alias[1]), &(arr[size_old]), true, old_arg))
        str = my_str_join(" ", (const char **)(arr));
    nfree_array(2, arr, alias);
    return str;
}

static char **handle_alias(char *alias_val, char **old_arg,
    void *array[], int *last_return)
{
    jobs_t **jobs = (jobs_t **)array[3];
    char *new_command = build_alias_command(alias_val,
        (const char **)(old_arg));
    char **result = NULL;

    free_array(old_arg);
    if (!new_command)
        return (char **)array[0];
    result = parse_command(new_command, array, last_return, jobs);
    free(new_command);
    return result;
}

static bool must_alias_be_applied(char **args, const char *alias)
{
    char **splited = transform_to_string_array(alias, " \t");

    if (!args)
        return false;
    if (!splited)
        return false;
    if (strcmp(args[0], (const char *)(splited[0])) != 0)
        return false;
    for (size_t i = 1; splited[i] != NULL; ++i)
        if (!my_is_in_str_array((const char *)(splited[i]),
                (const char **)(args))) {
            free_array(splited);
            return true;
        }
    free_array(splited);
    return false;
}

char **check_alias_expansion(char **arg, void *array[], int *last_return)
{
    alias_t **list = (alias_t **)array[1];
    char *val = get_alias(*list, arg[0]);
    int name_len = strlen(arg[0]);;

    if (!val || strcmp(arg[0], "alias") == 0)
        return NULL;
    if (must_alias_be_applied(arg, (const char *)(val)))
        return handle_alias(val, arg, array, last_return);
    return NULL;
}
