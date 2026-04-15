/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Bonus (display custom prompt)
*/

#include <string.h>
#include "functions.h"

static char *get_user(char **copy_env)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        if (strncmp((const char *)(copy_env[i]), "USER=", 5) == 0)
            return &copy_env[i][5];
    }
    return NULL;
}

static char *get_home(char **copy_env)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        if (strncmp((const char *)(copy_env[i]), "HOME=", 5) == 0)
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
    printf("%s:", username);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (home && strncmp((const char *)(cwd), (const char *)(home),
                strlen((const char *)(home))) == 0)
            printf("~%s", cwd + strlen((const char *)(home)));
        else
            printf("%s", cwd);
    }
    printf("> ");
}
