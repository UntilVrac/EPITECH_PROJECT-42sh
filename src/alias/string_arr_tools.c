/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** This file contains string arr tool functions
*/

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

bool my_is_in_str_array(const char *str, const char **arr)
{
    if (!str || !arr)
        return false;
    for (size_t i = 0; arr[i] != NULL; ++i)
        if (strcmp(str, arr[i]) == 0)
            return true;
    return false;
}

bool cpy_string_arr(const char **src, char **dest,
    bool if_not_in_arr, const char **old_cmd)
{
    bool cpy = true;
    size_t pos = 0;

    if (!src || !dest || !old_cmd)
        return false;
    for (size_t i = 0; src[i] != NULL; ++i) {
        if (if_not_in_arr)
            cpy = (my_is_in_str_array(src[i], old_cmd) == 0);
        if (cpy) {
            dest[pos] = strdup(src[i]);
            pos += 1;
        }
        if (cpy && !(dest[pos - 1]))
            return false;
    }
    return true;
}

void my_show_string_arr(const char **arr)
{
    fprintf(stderr, "{\n");
    for (size_t i = 0; arr[i] != 0; ++i)
        fprintf(stderr, "    %s\n", arr[i]);
    fprintf(stderr, "}\n");
}
