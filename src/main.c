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
#include "history.h"
// #include "graphic.h"

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
        if (strncmp(env[j], "PATH=", 5) == 0)
            copy_env[j] = my_str_n_concat(4, env[j], ":", getcwd(NULL, 0),
            "/bonus");
        else
            copy_env[j] = strdup((const char *)(env[j]));
    }
    copy_env[i] = NULL;
    return copy_env;
}

char **process_line(void *data[], char **copy_env, int *last_return,
    jobs_t **jobs)
{
    history_t **history = (history_t **)data[2];
    char **commands = NULL;
    char *line = (char *)data[0];
    int len = strlen((const char *)(line));

    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    line = handle_backticks(line, last_return, jobs, copy_env);
    commands = split_semicolon(line);
    commands = check_exclamation_mark(history, commands);
    if (line)
        free(line);
    if (!commands)
        return copy_env;
    for (int i = 0; commands[i] != NULL; i++)
        copy_env = parse_command(commands[i], (void *[]){copy_env, data[1],
                commands, jobs, history}, last_return, jobs);
    free_array(commands);
    return copy_env;
}

void print_exit(void)
{
    if (isatty(STDIN_FILENO))
        printf("exit\n");
}

static void clean_exit(jobs_t *jobs, char *line, alias_t *alias_list,
    history_t **history)
{
    print_exit();
    free_alias_list(alias_list);
    free_jobs_struct(jobs);
    free_history_struct(history);
    free(line);
}

static void read_input(char ***copy_env, int *last_return)
{
    char *line = NULL;
    size_t line_length = 0;
    jobs_t *jobs = init_jobs_struct();
    alias_t *alias_list = NULL;
    history_t **history = NULL;

    if (!jobs) {
        free_array(*copy_env);
        exit(84);
    }
    for (size_t current = 0; 1;) {
        *copy_env = run_precmd((void *[]){*copy_env, last_return, &jobs,
                history}, last_return, &alias_list);
        display_custom_prompt(*copy_env);
        if (getline(&line, &line_length, stdin) < 0)
            break;
        history = add_command_to_history(history, line, &current);
        *copy_env = process_line((void *[]){line, &alias_list, history},
            *copy_env, last_return, &jobs);
        line = NULL;
    }
    clean_exit(jobs, line, alias_list, history);
}

// static int parse_arguments(int ac, const char **av, bool *close_fd,
//     const char **env)
// {
//     if (ac > MAX_ARG_NBR) {
//         print_error("42sh", TOO_M_ARGS, env);
//         return EPITECH_FAILURE;
//     }
//     if (ac == 0)
//         return EPITECH_SUCCESS;
//     if (strcmp(*av, CLOSE_FD_FLAG) == 0) {
//         *close_fd = true;
//         return EPITECH_SUCCESS;
//     }
//     print_error(*av, INVALID_ARG, env);
//     return EPITECH_FAILURE;
// }
int main(int argc, const char **argv, char **env)
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
