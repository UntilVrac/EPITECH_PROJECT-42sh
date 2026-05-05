/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Pipe handling
*/

#include <string.h>
#include "functions.h"
#include "lang.h"
#include "alias.h"

static int skip_spaces_tabs(char *line, int i)
{
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    return i;
}

static int skip_to_next(char *line, int i)
{
    while (line[i] && line[i] != '|')
        i++;
    return i;
}

int pipe_syntax_error(char *line)
{
    int i = 0;
    int found_command = 0;

    while (line[i]) {
        i = skip_spaces_tabs(line, i);
        if (line[i] == '|' && found_command == 0)
            return -1;
        if (line[i] == '|' && found_command == 1) {
            found_command = 0;
            i++;
            continue;
        }
        if (line[i] != '\0' && line[i] != '\n') {
            found_command = 1;
            i = skip_to_next(line, i);
        }
    }
    if (found_command == 0)
        return -1;
    return 0;
}

static void redirect_pipe_fd(int in_fd, int out_fd)
{
    if (in_fd != STDIN_FILENO) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
}

static char **get_arg_ready(char *command, void *data[])
{
    char **arg = transform_to_string_array((const char *)(command), " \t");
    alias_t **list = (alias_t **)data[1];
    char *expanded_arg = NULL;
    char *val = NULL;

    arg = apply_globbings_on_args(arg, (const char **)(data[0]));
    if (!arg || !arg[0])
        return arg;
    val = get_alias(*list, arg[0]);
    if (val && strcmp(arg[0], val) != 0) {
        expanded_arg = build_alias_command(val, arg);
        free_array(arg);
        arg = transform_to_string_array((const char *)expanded_arg, " \t");
        arg = apply_globbings_on_args(arg, (const char **)(data[0]));
        free(expanded_arg);
    }
    return arg;
}

static void exec_pipe_child(char *command, int in_fd,
    int out_fd, void *data[])
{
    char **arg = NULL;
    char *path = NULL;
    int builtin_return = 0;

    redirect_pipe_fd(in_fd, out_fd);
    if (apply_redirection((const char *)(command),
            (const char **)(data[0])) == -1)
        exit(1);
    arg = get_arg_ready(command, data);
    if (execute_builtin(arg, (char **)data[0], &builtin_return, NULL) != NULL) {
        free_array(arg);
        exit(builtin_return);
    }
    path = get_command_path(arg[0], (char **)data[0]);
    if (path == NULL)
        exit(1);
    execute_child(arg, path, (char **)data[0]);
}

static int setup_pipe(char **commands, int index, int *pipe_fd)
{
    if (commands[index + 1] == NULL)
        return STDOUT_FILENO;
    if (pipe(pipe_fd) == -1)
        return -1;
    return pipe_fd[1];
}

static pid_t exec_fork_pipe(char **commands, int index, int *prev_fd,
    void *data[])
{
    int pipe_fd[2];
    pid_t pid;
    int out_fd = setup_pipe(commands, index, pipe_fd);

    if (out_fd == -1)
        return -1;
    pid = fork();
    if (pid == 0) {
        if (commands[index + 1] != NULL)
            close(pipe_fd[0]);
        exec_pipe_child(commands[index], *prev_fd, out_fd, data);
    }
    if (*prev_fd != STDIN_FILENO)
        close(*prev_fd);
    if (commands[index + 1] != NULL) {
        close(pipe_fd[1]);
        *prev_fd = pipe_fd[0];
    }
    return pid;
}

static void wait_for_all(pid_t last_pid, int *last_return, int nb_commands,
    const char **env)
{
    int status1 = 0;
    int status2 = 0;
    int pipe_error = 0;

    waitpid(last_pid, &status1, 0);
    if (WIFEXITED(status1))
        *last_return = WEXITSTATUS(status1);
    else if (WIFSIGNALED(status1))
        handle_signal_error(status1, last_return, env);
    for (int i = 0; i < nb_commands - 1; i++) {
        status2 = 0;
        wait(&status2);
        if (WIFEXITED(status2) && WEXITSTATUS(status2) != 0)
            pipe_error = 1;
        if (WIFSIGNALED(status2))
            pipe_error = 1;
    }
    if (*last_return == 0 && pipe_error == 1)
        *last_return = 1;
}

void handle_pipe(char *line, char **copy_env,
    int *last_return, alias_t **alias_list)
{
    char *copy_line = strdup((const char *)(line));
    char **commands = transform_to_string_array(copy_line, "|");
    int prev_fd = STDIN_FILENO;
    pid_t last_pid = 0;
    int nb_commands = 0;
    void *data[2] = {copy_env, alias_list};

    free(copy_line);
    if (!commands)
        return;
    while (commands[nb_commands])
        nb_commands++;
    for (int i = 0; commands[i] != NULL; i++)
        last_pid = exec_fork_pipe(commands, i, &prev_fd, data);
    wait_for_all(last_pid, last_return, nb_commands, (const char **)(copy_env));
    free_array(commands);
}
