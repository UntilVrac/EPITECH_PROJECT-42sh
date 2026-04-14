/*
** EPITECH PROJECT, 2026
** MINISHELL 2
** File description:
** Bonus (display custom prompt)
*/

#include "../include/functions.h"

static char *get_user(char **copy_env)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        if (my_strncmp(copy_env[i], "USER=", 5) == 0)
            return &copy_env[i][5];
    }
    return NULL;
}

static char *get_home(char **copy_env)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        if (my_strncmp(copy_env[i], "HOME=", 5) == 0)
            return &copy_env[i][5];
    }
    return NULL;
}

void display_custom_prompt(char **copy_env)
{
    char *username = get_user(copy_env);
    char cwd[1024];
    char *home = get_home(copy_env);

    if (!isatty(0) || !username)
        return;
    if ('a' <= username[0] && username[0] <= 'z')
        username[0] -= 32;
    write(1, username, my_strlen(username));
    write(1, ":", 1);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (home && my_strncmp(cwd, home, my_strlen(home)) == 0) {
            write(1, "~", 1);
            write(1, cwd + my_strlen(home), my_strlen(cwd + my_strlen(home)));
        } else {
            write(1, cwd, my_strlen(cwd));
        }
    }
    write(1, "> ", 2);
}
