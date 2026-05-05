/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Minishell 2 project
*/

#include <string.h>
#include "functions.h"
#include "lang.h"
#include "alias.h"

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

char **process_line(void *data[], char **copy_env, int *last_return,
    jobs_t **jobs)
{
    char **commands = NULL;
    char *line = (char *)data[0];
    int len = strlen((const char *)(line));

    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    line = handle_backticks(line, last_return, jobs, copy_env);
    commands = split_semicolon(line);
    if (line)
        free(line);
    if (!commands)
        return copy_env;
    for (int i = 0; commands[i] != NULL; i++)
        copy_env = parse_command(commands[i],
            (void *[]) {copy_env, data[1], commands, jobs}, last_return, jobs);
    free_array(commands);
    return copy_env;
}

void print_exit(void)
{
    if (isatty(STDIN_FILENO))
        printf("exit\n");
}

static void clean_exit(jobs_t *jobs, char *line, alias_t *alias_list)
{
    print_exit();
    free_alias_list(alias_list);
    free_jobs_struct(jobs);
    free(line);
}

static void read_input(char ***copy_env, int *last_return)
{
    char *line = NULL;
    size_t line_length = 0;
    jobs_t *jobs = init_jobs_struct();
    alias_t *alias_list = NULL;
    void *data[2];

    if (!jobs) {
        free_array(*copy_env);
        exit(84);
    }
    while (1) {
        display_custom_prompt(*copy_env);
        if (getline(&line, &line_length, stdin) == -1)
            break;
        data[0] = line;
        data[1] = &alias_list;
        *copy_env = process_line(data, *copy_env, last_return, &jobs);
        line = NULL;
    }
    clean_exit(jobs, line, alias_list);
}

int main(int argc, char **argv, char **env)
{
    char **copy_env = get_env_copy(env);
    int last_return = 0;

    init_env_dir((const char **)(copy_env));
    (void)argc;
    (void)argv;
    for (size_t i = 0; ignored_signals[i] != 0; ++i)
        signal(ignored_signals[i], SIG_IGN);
    read_input(&copy_env, &last_return);
    free_array(copy_env);
    return last_return;
}
