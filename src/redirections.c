/*
** EPITECH PROJECT, 2026
** MINISHELL 2
** File description:
** Redirections handling
*/

#include "../include/functions.h"

static int ambiguous_redirect(char *command)
{
    int nb_input = 0;
    int nb_output = 0;
    const char *error1 = "Ambiguous input redirect.\n";
    const char *error2 = "Ambiguous output redirect.\n";

    for (int i = 0; command[i] != '\0'; i++) {
        if (command[i] == '<' && (i == 0 || command[i - 1] != '<'))
            nb_input++;
        if (command[i] == '>' && (i == 0 || command[i - 1] != '>'))
            nb_output++;
    }
    if (nb_input > 1) {
        write(2, error1, my_strlen((char *)error1));
        return -1;
    }
    if (nb_output > 1) {
        write(2, error2, my_strlen((char *)error2));
        return -1;
    }
    return 0;
}

static int missing_name(char *command, int index)
{
    const char *error = "Missing name for redirect.\n";

    while (command[index] == ' ' || command[index] == '\t')
        index++;
    if (command[index] == '\0' || command[index] == '|' ||
        command[index] == '>' || command[index] == '<') {
        write(2, error, my_strlen((char *)error));
        return -1;
    }
    return 0;
}

static int redirection_syntax_error(char *command)
{
    int index = 0;

    if (ambiguous_redirect(command) == -1)
        return -1;
    for (int i = 0; command[i] != '\0'; i++) {
        if (command[i] != '>' && command[i] != '<')
            continue;
        index = i + 1;
        if (command[i] == command[index])
            index++;
        if (missing_name(command, index) == -1)
            return -1;
        i = index - 1;
    }
    return 0;
}

static char *get_filename(char *str)
{
    int i = 0;
    char *filename = NULL;

    while (str[i] == ' ' || str[i] == '\t')
        i++;
    filename = my_strdup(&str[i]);
    if (!filename)
        return NULL;
    for (int j = 0; filename[j] != '\0'; j++)
        if (filename[j] == ' ' || filename[j] == '\t') {
            filename[j] = '\0';
            break;
        }
    return filename;
}

static int get_output_fd(char *ptr, char **filename)
{
    int fd = 0;

    if (*(ptr + 1) == '>') {
        *filename = get_filename(ptr + 2);
        if (!*filename)
            return -1;
        fd = open(*filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    } else {
        *filename = get_filename(ptr + 1);
        if (!*filename)
            return -1;
        fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    return fd;
}

static int output_redirection(char *command)
{
    char *ptr = my_strchr(command, '>');
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

static int get_input_fd(char *ptr, char **filename)
{
    int fd = 0;

    *filename = get_filename(ptr + 1);
    if (!*filename)
        return -1;
    fd = open(*filename, O_RDONLY, 0644);
    return fd;
}

static int input_redirection(char *command)
{
    char *ptr = my_strchr(command, '<');
    char *filename = NULL;
    int fd = 0;

    if (!ptr || *(ptr + 1) == '<')
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

int apply_redirection(char *command)
{
    if (redirection_syntax_error(command) == -1)
        return -1;
    if (input_redirection(command) == -1)
        return -1;
    if (output_redirection(command) == -1)
        return -1;
    return 0;
}
