/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Minishell 2 project
*/

#include <string.h>
#include "functions.h"
#include "lang.h"

void my_putchar(char c)
{
    write(1, &c, 1);
}

void my_putstr(char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        my_putchar(str[i]);
    }
}

void free_array(char **arg)
{
    if (arg == NULL)
        return;
    for (int i = 0; arg[i] != NULL; i++) {
        free(arg[i]);
    }
    free(arg);
}

static char **get_env_copy(char **env)
{
    int i = 0;
    char **copy_env = NULL;

    while (env[i])
        i++;
    copy_env = malloc(sizeof(char *) * (i + 1));
    if (!copy_env) {
        perror("malloc copy_env");
        exit(84);
    }
    for (int j = 0; j < i; j++) {
        copy_env[j] = strdup((const char *)(env[j]));
    }
    copy_env[i] = NULL;
    return copy_env;
}

static char **process_line(char *line, char **copy_env, int *last_return)
{
    char **commands = NULL;
    int len = strlen((const char *)(line));

    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    commands = transform_to_string_array(line, ";");
    if (line)
        free(line);
    if (!commands)
        return copy_env;
    for (int i = 0; commands[i] != NULL; i++)
        copy_env = parse_command(commands[i], copy_env, last_return, commands);
    free_array(commands);
    return copy_env;
}

int main(int argc, char **argv, char **env)
{
    char *line = NULL;
    char **copy_env = get_env_copy(env);
    size_t line_length = 0;
    int last_return = 0;

    init_env_dir((const char **)(copy_env));
    (void)argc;
    (void)argv;
    signal(SIGINT, SIG_IGN);
    while (1) {
        display_custom_prompt(copy_env);
        if (getline(&line, &line_length, stdin) == -1)
            break;
        copy_env = process_line(line, copy_env, &last_return);
        line = NULL;
    }
    free(line);
    free_array(copy_env);
    return last_return;
}
