/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Execution and path handling functions
*/

#include <string.h>
#include "functions.h"
#include "lang.h"

static char *get_path(char **copy_env)
{
    for (int i = 0; copy_env[i] != NULL; i++) {
        if (strncmp((const char *)(copy_env[i]), "PATH=", 5) == 0) {
            return &copy_env[i][5];
        }
    }
    return NULL;
}

static char *build_full_path(char *director, char *command)
{
    char *full_path = malloc(strlen((const char *)(director)) +
        strlen((const char *)(command)) + 2);

    if (!full_path) {
        perror("malloc error full_path");
        return NULL;
    }
    full_path[0] = '\0';
    strcat(full_path, (const char *)(director));
    strcat(full_path, "/");
    strcat(full_path, (const char *)(command));
    return full_path;
}

static char *check_path(char **directors, char *command, char *path_copy)
{
    char *full_path = NULL;

    for (int i = 0; directors[i] != NULL; i++) {
        if (strlen((const char *)(directors[i])) == 0)
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

    if (command && (strchr((const char *)(command), '/')))
        return strdup((const char *)(command));
    path = get_path(copy_env);
    if (path == NULL)
        return NULL;
    path_copy = strdup((const char *)(path));
    directors = transform_to_string_array(path_copy, ":");
    if (!directors) {
        free(path_copy);
        return NULL;
    }
    return check_path(directors, command, path_copy);
}

void handle_signal_error(int status, int *last_return, const char **env)
{
    int sig = WTERMSIG(status);
    char *message = strsignal(sig);

    if (sig == SIGINT) {
        write(1, "\n", 1);
        *last_return = 1;
        return;
    }
    if (WCOREDUMP(status) && sig == SIGSEGV)
        print_error(NULL, SEG_FAULT, env);
    else if (sig == SIGSEGV)
        print_error(NULL, SEG_FAULT_CD, env);
    if (sig != SIGSEGV && message) {
        fprintf(stderr, "%s", message);
        if (WCOREDUMP(status))
            fprintf(stderr, " (core dumped)");
        fprintf(stderr, "\n");
    }
    *last_return = 128 + sig;
}

void execute_child(char **arg, char *path, char **copy_env)
{
    signal(SIGINT, SIG_DFL);
    if (execve(path, arg, copy_env) == -1) {
        if (errno == ENOEXEC) {
            print_error((const char *)(arg[0]), NOT_EXEC,
                (const char **)(copy_env));
            free(path);
            exit(1);
        }
        if (errno == EACCES)
            print_error((const char *)(arg[0]), PERM_DENIED,
                (const char **)(copy_env));
        else
            print_error((const char *)(arg[0]), CMD_NOT_FOUND,
                (const char **)(copy_env));
        free(path);
        exit(1);
    }
}

static void execute_parent(pid_t pid, char *path, int *last_return,
    const char **env)
{
    int status = 0;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        *last_return = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        handle_signal_error(status, last_return, (const char **)(env));
    }
    free(path);
}

static void execute_redirection_child(char *command_copy, char **arg,
    char *path, char **copy_env)
{
    if (apply_redirection(command_copy, (const char **)(copy_env)) == -1)
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
    char *command_copy = strdup((const char *)(command));
    char *tmp_copy = strdup((const char *)(command));
    char **arg = transform_to_string_array(tmp_copy, " \t");
    char *path = get_command_path(arg[0], copy_env);
    pid_t pid = 0;

    if (path == NULL) {
        print_error((const char *)(arg[0]), CMD_NOT_FOUND,
            (const char **)(copy_env));
        *last_return = 1;
    } else {
        pid = fork();
        if (pid == 0)
            execute_redirection_child(command_copy, arg, path, copy_env);
        else
            execute_parent(pid, path, last_return, (const char **)(copy_env));
    }
    free(tmp_copy);
    free_array(arg);
    free(command_copy);
}
