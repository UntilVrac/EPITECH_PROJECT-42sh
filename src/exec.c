/*
** EPITECH PROJECT, 2026
** MINISHELL 2
** File description:
** Execution and path handling functions
*/

#include "../include/functions.h"

static char *get_path(char **copy_env)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        if (my_strncmp(copy_env[i], "PATH=", 5) == 0) {
            return &copy_env[i][5];
        }
    }
    return NULL;
}

static char *build_full_path(char *director, char *command)
{
    char *full_path = malloc(my_strlen(director) + my_strlen(command) + 2);

    if (!full_path) {
        perror("malloc error full_path");
        return NULL;
    }
    full_path[0] = '\0';
    my_strcat(full_path, director);
    my_strcat(full_path, "/");
    my_strcat(full_path, command);
    return full_path;
}

static char *check_path(char **directors, char *command, char *path_copy)
{
    char *full_path = NULL;

    for (int i = 0; directors[i] != NULL; i++) {
        if (my_strlen(directors[i]) == 0)
            full_path = build_full_path(".", command);
        else
            full_path = build_full_path(directors[i], command);
        if (!full_path)
            break;
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            free_array(directors);
            return full_path;
        }
        free(full_path);
    }
    free(path_copy);
    free_array(directors);
    return NULL;
}

char *get_command_path(char *command, char **copy_env)
{
    char *path = NULL;
    char *path_copy = NULL;
    char **directors = NULL;

    if (command && (my_strchr(command, '/')))
        return my_strdup(command);
    path = get_path(copy_env);
    if (path == NULL)
        return NULL;
    path_copy = my_strdup(path);
    directors = transform_to_string_array(path_copy, ":");
    if (!directors) {
        free(path_copy);
        return NULL;
    }
    return check_path(directors, command, path_copy);
}

void handle_signal_error(int status, int *last_return)
{
    int sig = WTERMSIG(status);
    char *message = strsignal(sig);
    const char *dumped = " (core dumped)";

    if (sig == SIGINT) {
        write(1, "\n", 1);
        *last_return = 1;
        return;
    }
    if (sig == SIGSEGV)
        write(2, "Segmentation fault", 18);
    else if (message)
        write(2, message, my_strlen(message));
    if (WCOREDUMP(status))
        write(2, dumped, my_strlen((char *)dumped));
    write(2, "\n", 1);
    *last_return = 128 + sig;
}

void execute_child(char **arg, char *path, char **copy_env)
{
    const char *error1 = ": Permission denied.\n";
    const char *error2 = ": Command not found.\n";
    const char *error3 = ": Exec format error. Binary file not executable.\n";

    signal(SIGINT, SIG_DFL);
    if (execve(path, arg, copy_env) == -1) {
        write(2, arg[0], my_strlen(arg[0]));
        if (errno == ENOEXEC) {
            write(2, error3, my_strlen((char *)error3));
            free(path);
            exit(1);
        }
        if (errno == EACCES)
            write(2, error1, my_strlen((char *)error1));
        else
            write(2, error2, my_strlen((char *)error2));
        free(path);
        exit(1);
    }
}

static void execute_parent(pid_t pid, char *path, int *last_return)
{
    int status = 0;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        *last_return = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        handle_signal_error(status, last_return);
    }
    free(path);
}

static void execute_redirection_child(char *command_copy, char **arg,
    char *path, char **copy_env)
{
    if (apply_redirection(command_copy) == -1)
        exit(1);
    free_array(arg);
    arg = transform_to_string_array(command_copy, " \t");
    if (!arg || !arg[0])
        exit(0);
    execute_child(arg, path, copy_env);
    exit(0);
}

void execute_command(char *command, char **copy_env, int *last_return)
{
    char *command_copy = my_strdup(command);
    char *tmp_copy = my_strdup(command);
    char **arg = transform_to_string_array(tmp_copy, " \t");
    char *path = get_command_path(arg[0], copy_env);
    pid_t pid = 0;

    if (path == NULL) {
        write(2, arg[0], my_strlen(arg[0]));
        write(2, ": Command not found.\n", 21);
        *last_return = 1;
    } else {
        pid = fork();
        if (pid == 0)
            execute_redirection_child(command_copy, arg, path, copy_env);
        else
            execute_parent(pid, path, last_return);
    }
    free(tmp_copy);
    free_array(arg);
    free(command_copy);
}
