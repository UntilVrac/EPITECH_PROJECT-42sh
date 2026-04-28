/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Globbings handling
*/

#include <string.h>
#include <glob.h>
#include <stddef.h>
#include "lang.h"
#include "functions.h"

char *get_path_with_home_dir(const char *str_init, const char **env)
{
    char *str_traited = NULL;
    int home_index = 0;
    char **env_line = NULL;

    if (strncmp(str_init, "~/", 2) != 0)
        return strdup(str_init);
    home_index = get_env_var_index(env, "HOME");
    if (home_index < 0)
        return strdup(str_init);
    env_line = my_split_str(env[home_index], '=');
    if (env_line == NULL)
        return strdup(str_init);
    if (my_word_array_len((const char **)(env_line)) < 2) {
        free_array(env_line);
        return strdup(str_init);
    }
    str_traited = my_str_concat(env_line[1], &(str_init[1]));
    free_array(env_line);
    return str_traited;
}

static int get_possibilities(char **str)
{
    glob_t glob_struct;
    char *tmp = NULL;

    glob_struct.gl_pathv = NULL;
    if (glob((const char *)(*str), GLOB_FLAGS, NULL, &glob_struct) != 0) {
        globfree(&glob_struct);
        return EPITECH_FAILURE;
    }
    tmp = my_str_join(" ", (const char **)(glob_struct.gl_pathv));
    globfree(&glob_struct);
    if (tmp == NULL)
        return EPITECH_FAILURE;
    free(*str);
    *str = tmp;
    return EPITECH_SUCCESS;
}

int get_paths_with_globbings(char **str, const char **env)
{
    char *pattern = get_path_with_home_dir((const char *)(*str), env);

    if (pattern == NULL)
        return EPITECH_FAILURE;
    free(*str);
    *str = pattern;
    return get_possibilities(str);
}

char **apply_globbings_on_args(char **args, const char **env)
{
    char *tmp = NULL;

    if (args == NULL)
        return NULL;
    for (size_t i = 0; args[i] != NULL; ++i)
        get_paths_with_globbings(&(args[i]), env);
    tmp = my_str_join(" ", (const char **)(args));
    free_array(args);
    if (tmp == NULL)
        return NULL;
    args = my_split_str((const char *)(tmp), ' ');
    free(tmp);
    return args;
}
