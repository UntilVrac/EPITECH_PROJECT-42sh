/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** This file containes tools functions
*/

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "functions.h"
#include "lang.h"

char *cut_ending_char(char *buffer, char c)
{
    int l = 0;

    if (!buffer)
        return buffer;
    l = strlen((const char *)(buffer));
    if (buffer[l - 1] == c)
        buffer[l - 1] = '\0';
    return buffer;
}

bool str_isnum(const char *str, int *val)
{
    char *tmp = NULL;
    int nbr = strtol(str, &tmp, 10);

    if (val)
        *val = nbr;
    if (!tmp || tmp[0])
        return false;
    return true;
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
