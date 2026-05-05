/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Backticks handling
*/

#include <string.h>
#include "functions.h"
#include "lang.h"
#include "jobs.h"

static void finish_res(int len, char *res)
{
    if (len > 0 && res[len - 1] == '\n')
        res[len - 1] = '\0';
    for (int i = 0; res[i]; i++)
        if (res[i] == '\n')
            res[i] = ' ';
}

static char *read_output(int fd)
{
    char buffer[1024];
    char *res = strdup("");
    char *tmp = NULL;
    int n = read(fd, buffer, 1023);

    while (n > 0) {
        buffer[n] = '\0';
        tmp = malloc(strlen(res) + n + 1);
        if (!tmp)
            return res;
        strcpy(tmp, res);
        strcat(tmp, buffer);
        free(res);
        res = tmp;
        n = read(fd, buffer, 1023);
    }
    finish_res(strlen(res), res);
    return res;
}

static void cmd_exec(char **args, char **env)
{
    char *path = get_command_path(args[0], env);

    if (!path) {
        print_error(args[0], CMD_NOT_FOUND, (const char **)env);
        exit(1);
    }
    if (execve(path, args, env) == -1) {
        perror(args[0]);
        exit(1);
    }
}

static void process_command(char *cmd, char **env, int *last_return)
{
    char *cmd_copy = strdup(cmd);
    char **args = transform_to_string_array(cmd_copy, " \t");

    if (!args || !args[0]) {
        free(cmd_copy);
        exit(0);
    }
    args = apply_globbings_on_args(args, (const char **)(env));
    if (apply_redirection(cmd, (const char **)env) == -1)
        exit(1);
    for (size_t i = 0; builtins_functions[i].name; i++)
        if (!strcmp(args[0], builtins_functions[i].name)) {
            builtins_functions[i].ptr(args, env, last_return);
            exit(*last_return);
        }
    cmd_exec(args, env);
}

static void exec_child(int pipe_fds[2], char *cmd, int *last_return,
    char **env)
{
    close(pipe_fds[0]);
    dup2(pipe_fds[1], STDOUT_FILENO);
    dup2(pipe_fds[1], STDERR_FILENO);
    close(pipe_fds[1]);
    process_command(cmd, env, last_return);
}

static char *exec_cmd(char *cmd, int *last_return, char **env)
{
    int pipe_fds[2];
    pid_t pid;
    char *output = NULL;
    int status;

    if (pipe(pipe_fds) == -1)
        return strdup("");
    pid = fork();
    if (pid == 0)
        exec_child(pipe_fds, cmd, last_return, env);
    close(pipe_fds[1]);
    output = read_output(pipe_fds[0]);
    close(pipe_fds[0]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        *last_return = WEXITSTATUS(status);
    return output;
}

static char *construct_str(char *line, char *output, int start, int end)
{
    int len = start + strlen(output) + strlen(line + end + 1);
    char *res = malloc(len + 1);

    if (!res)
        return line;
    strncpy(res, line, start);
    res[start] = '\0';
    strcat(res, output);
    strcat(res, line + end + 1);
    free(output);
    free(line);
    return res;
}

static char *replace_str(char *line, int start, int *last_return, char **env)
{
    int end = -1;
    char *cmd = NULL;
    char *output = NULL;

    for (int i = start + 1; line[i]; i++)
        if (line[i] == '`') {
            end = i;
            break;
        }
    if (end == -1)
        return line;
    cmd = strndup(line + start + 1, end - start - 1);
    output = exec_cmd(cmd, last_return, env);
    free(cmd);
    return construct_str(line, output, start, end);
}

char *handle_backticks(char *line, int *last_return, jobs_t **jobs,
    char **env)
{
    (void)jobs;
    for (int i = 0; line[i]; i++)
        if (line[i] == '`') {
            line = replace_str(line, i, last_return, env);
            i = -1;
        }
    return line;
}
