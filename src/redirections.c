/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Redirections handling
*/

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"
#include "lang.h"

static int ambiguous_redirect(const char *command, const char **env)
{
    int nb_input = 0;
    int nb_output = 0;

    for (int i = 0; command[i] != '\0'; i++) {
        if (command[i] == '<' && (i == 0 || command[i - 1] != '<'))
            nb_input++;
        if (command[i] == '>' && (i == 0 || command[i - 1] != '>'))
            nb_output++;
    }
    if (nb_input > 1) {
        print_error(NULL, AMB_INPUT_RED, env);
        return -1;
    }
    if (nb_output > 1) {
        print_error(NULL, AMB_INPUT_RED, env);
        return -1;
    }
    return 0;
}

static int missing_name(const char *command, int index, const char **env)
{
    while (command[index] == ' ' || command[index] == '\t')
        index++;
    if (command[index] == '\0' || command[index] == '|' ||
        command[index] == '>' || command[index] == '<') {
        print_error(NULL, MISS_NAME_FOR_RED, env);
        return -1;
    }
    return 0;
}

static int redirection_syntax_error(const char *command, const char **env)
{
    int index = 0;

    if (ambiguous_redirect(command, env) == -1)
        return -1;
    for (int i = 0; command[i] != '\0'; i++) {
        if (command[i] != '>' && command[i] != '<')
            continue;
        index = i + 1;
        if (command[i] == command[index])
            index++;
        if (missing_name(command, index, env) == -1)
            return -1;
        i = index - 1;
    }
    return 0;
}

static char *get_filename(const char *str)
{
    int i = 0;
    char *filename = NULL;

    while (str[i] == ' ' || str[i] == '\t')
        i++;
    filename = strdup((const char *)(&str[i]));
    if (!filename)
        return NULL;
    for (int j = 0; filename[j] != '\0'; j++)
        if (filename[j] == ' ' || filename[j] == '\t') {
            filename[j] = '\0';
            break;
        }
    return filename;
}

static int get_output_fd(const char *ptr, char **filename)
{
    int mode = O_WRONLY | O_CREAT;
    int jmp = 1;

    if (*(ptr + 1) == '>') {
        jmp = 2;
        mode = mode | O_APPEND;
    } else
        mode = mode | O_TRUNC;
    *filename = get_filename((const char *)(ptr + jmp));
    if (!*filename)
        return -1;
    return open((const char *)(*filename), mode, 0644);
}

static int output_redirection(const char *command)
{
    char *ptr = strchr(command, '>');
    char *filename = NULL;
    int fd = 0;

    if (!ptr)
        return 0;
    fd = get_output_fd(ptr, &filename);
    *ptr = '\0';
    if (fd == -1) {
        perror(filename);
        free(filename);
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    free(filename);
    return 0;
}

static int double_stdin_redirection(const char *stop_word)
{
    int pipe_fd[2] = {0, 0};
    bool loop = true;
    char *buffer = NULL;
    size_t size = 0;

    if (pipe(pipe_fd) != 0)
        return -1;
    while (loop) {
        write(STDOUT_FILENO, "> ", 2);
        loop = false;
        if (getline(&buffer, &size, stdin) > 0) {
            dprintf(pipe_fd[1], "%s", buffer);
            loop = (strcmp((const char *)(cut_ending_char(buffer, '\n')),
                    stop_word) != 0);
        }
    }
    free(buffer);
    close(pipe_fd[1]);
    return pipe_fd[0];
}

static int get_input_fd(char *ptr, char **filename)
{
    int fd = 0;
    bool double_redirect = (*(ptr + 1) == '<');

    if (double_redirect)
        ptr += 1;
    *filename = get_filename(ptr + 1);
    if (!*filename)
        return -1;
    if (double_redirect)
        return double_stdin_redirection((const char *)(*filename));
    fd = open(*filename, O_RDONLY, 0644);
    return fd;
}

static int input_redirection(const char *command)
{
    char *ptr = strchr(command, '<');
    char *filename = NULL;
    int fd = 0;

    if (!ptr)
        return 0;
    fd = get_input_fd(ptr, &filename);
    *ptr = '\0';
    if (fd == -1) {
        perror(filename);
        free(filename);
        return -1;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    free(filename);
    return 0;
}

int apply_redirection(const char *command, const char **env)
{
    if (redirection_syntax_error(command, env) == -1)
        return -1;
    if (input_redirection(command) == -1)
        return -1;
    if (output_redirection(command) == -1)
        return -1;
    return 0;
}
