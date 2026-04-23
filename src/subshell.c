/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Handling parentheses logic
*/

#include "functions.h"
#include "lang.h"

static char *get_subshell_content(const char *command)
{
    int i = 0;
    int start_pos = 0;
    int depth = 0;

    while (command[i] == ' ' || command[i] == '\t')
        i++;
    if (command[i] != '(')
        return NULL;
    start_pos = i + 1;
    for (; command[i] != '\0'; i++) {
        if (command[i] == '(')
            depth++;
        if (command[i] == ')')
            depth--;
        if (depth == 0)
            return my_substring(command, start_pos, i - start_pos);
    }
    return NULL;
}

static void exec_child_subshell(char *content, char **copy_env,
    int *last_return, jobs_t **jobs)
{
    char *subshell_command = strdup(content);

    if (subshell_command)
        process_line(subshell_command, copy_env, last_return, jobs);
    exit(*last_return);
}

static void exec_parent_subshell(pid_t pid, int *last_return, char **copy_env)
{
    int status = 0;

    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status)) {
        *last_return = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        handle_signal_error(status, last_return, (const char **)(copy_env));
    }
}

static char **exec_subshell(char *content, char **copy_env,
    int *last_return, jobs_t **jobs)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return copy_env;
    }
    if (pid == 0) {
        exec_child_subshell(content, copy_env, last_return, jobs);
    } else {
        exec_parent_subshell(pid, last_return, copy_env);
    }
    return copy_env;
}

int check_subshell(char *command, char **copy_env,
    int *last_return, jobs_t **jobs)
{
    char *subshell_content = get_subshell_content(command);

    if (subshell_content == NULL)
        return 0;
    exec_subshell(subshell_content, copy_env, last_return, jobs);
    free(subshell_content);
    return 1;
}

static void update_depth(char character, int *depth)
{
    if (character == '(')
        (*depth)++;
    if (character == ')')
        (*depth)--;
}

static int count_semicolons(char *line)
{
    int count = 1;
    int depth = 0;

    if (!line)
        return 0;
    for (int i = 0; line[i] != '\0'; i++) {
        update_depth(line[i], &depth);
        if (depth == 0 && line[i] == ';')
            count++;
    }
    return count;
}

char **split_semicolon(char *line)
{
    int depth = 0;
    int start_pos = 0;
    int index = 0;
    char **commands = malloc(sizeof(char *) * (count_semicolons(line) + 1));

    if (!commands || !line)
        return NULL;
    for (int i = 0; line[i] != '\0'; i++) {
        update_depth(line[i], &depth);
        if (depth == 0 && line[i] == ';') {
            commands[index] = my_substring(line, start_pos, i - start_pos);
            index++;
            start_pos = i + 1;
        }
    }
    commands[index] = my_substring(line, start_pos, strlen(line) - start_pos);
    index++;
    commands[index] = NULL;
    return commands;
}
