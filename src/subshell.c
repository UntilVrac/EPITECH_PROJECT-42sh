/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Handling parentheses logic
*/

#include "functions.h"
#include "lang.h"
#include "alias.h"

void update_depth(char character, int *depth)
{
    if (character == '(')
        (*depth)++;
    if (character == ')')
        (*depth)--;
}

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
        update_depth(command[i], &depth);
        if (depth == 0)
            return my_substring(command, start_pos, i - start_pos);
    }
    return NULL;
}

static void exec_child_subshell(char *content, char **copy_env,
    int *last_return, jobs_t **jobs)
{
    char *subshell_command = strdup(content);
    alias_t *alias_list = NULL;
    void *data[2];

    if (!subshell_command)
        exit(1);
    data[0] = subshell_command;
    data[1] = &alias_list;
    process_line(data, copy_env, last_return, jobs);
    free_alias_list(alias_list);
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
        perror("fork error exec_subshell");
        return copy_env;
    }
    if (pid == 0) {
        exec_child_subshell(content, copy_env, last_return, jobs);
    } else {
        exec_parent_subshell(pid, last_return, copy_env);
    }
    return copy_env;
}

static int is_empty(char *str)
{
    if (!str)
        return 1;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            return 0;
    return 1;
}

static int check_syntax(const char *command)
{
    int depth = 0;
    int i = 0;

    while (command[i] == ' ' || command[i] == '\t')
        i++;
    if (command[i] != '(')
        return 0;
    for (; command[i] != '\0'; i++) {
        update_depth(command[i], &depth);
        if (depth < 0)
            return -2;
    }
    if (depth > 0)
        return -1;
    return 1;
}

static int print_syntax_error(int syntax, int *last_return,
    char *subshell_content)
{
    const char *error1 = "Too many ('s.\n";
    const char *error2 = "Too many )'s.\n";
    const char *error3 = "Invalid null command.\n";

    if (syntax == -1)
        write(2, error1, strlen(error1));
    if (syntax == -2)
        write(2, error2, strlen(error2));
    if (syntax == -3)
        write(2, error3, strlen(error3));
    *last_return = 1;
    if (subshell_content)
        free(subshell_content);
    return 1;
}

int check_subshell(char *command, char **copy_env,
    int *last_return, jobs_t **jobs)
{
    int syntax = check_syntax(command);
    char *subshell_content = NULL;

    if (syntax < 0)
        return print_syntax_error(syntax, last_return, NULL);
    if (syntax == 0) {
        if (strchr(command, ')') != 0)
            return print_syntax_error(-2, last_return, NULL);
        return 0;
    }
    subshell_content = get_subshell_content(command);
    if (is_empty(subshell_content))
        return print_syntax_error(-3, last_return, subshell_content);
    exec_subshell(subshell_content, copy_env, last_return, jobs);
    free(subshell_content);
    return 1;
}
