/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Functions for managing the enviroment variables
*/

#include <string.h>
#include "functions.h"
#include "lang.h"

char *build_env_variable(char *name, char *value)
{
    int value_length = 0;
    char *new_var = NULL;

    if (value)
        value_length = strlen((const char *)(value));
    new_var = malloc(strlen((const char *)(name)) + value_length + 2);
    if (!new_var) {
        perror("malloc error build_env_variable");
        return NULL;
    }
    new_var[0] = '\0';
    strcat(new_var, (const char *)(name));
    strcat(new_var, "=");
    if (value)
        strcat(new_var, (const char *)(value));
    return new_var;
}

int get_env_var_index(const char **copy_env, char *name)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        if (strncmp((const char *)(copy_env[i]), (const char *)(name),
                strlen((const char *)(name))) == 0 &&
            copy_env[i][strlen((const char *)(name))] == '=')
            return i;
    }
    return -1;
}

int is_name_valid(char *name, const char **env)
{
    if (!((name[0] >= 'a' && name[0] <= 'z') ||
            (name[0] >= 'A' && name[0] <= 'Z'))) {
        print_error("setenv", VAR_NAME_M_BEG_W_LETTER, env);
        return -1;
    }
    for (int i = 0; name[i] != '\0'; i++) {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
                (name[i] >= 'A' && name[i] <= 'Z') ||
                (name[i] >= '0' && name[i] <= '9') ||
                (name[i] == '_'))) {
            print_error("setenv", VAR_NAME_M_CON_ALPHNUM_CHARS, env);
            return -1;
        }
    }
    return 0;
}

char **add_to_env(char **copy_env, char *new_var)
{
    int i = 0;
    char **new_env = NULL;

    while (copy_env[i])
        i++;
    new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env) {
        perror("malloc error new_env");
        return copy_env;
    }
    for (int j = 0; j < i; j++) {
        new_env[j] = copy_env[j];
    }
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    free(copy_env);
    return new_env;
}

void detele_env_var(char **copy_env, int index)
{
    free(copy_env[index]);
    for (int i = index; copy_env[i] != NULL; i++)
        copy_env[i] = copy_env[i + 1];
}
