/*
** EPITECH PROJECT, 2025
** G2 - Shell Programming - 42sh
** File description:
** this file countains string concatanation functions
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "lang.h"

char *my_str_concat(const char *str1, const char *str2)
{
    char *str_new;
    int size1 = strlen(str1);
    int size2 = strlen(str2);
    int index = 0;

    if (str1 == NULL || str2 == NULL)
        return NULL;
    str_new = malloc(sizeof(char) * (size1 + size2 + 1));
    if (str_new == NULL)
        return NULL;
    for (int i = 0; i < size1; ++i) {
        str_new[index] = str1[i];
        index += 1;
    }
    for (int i = 0; i < size2; ++i) {
        str_new[index] = str2[i];
        index += 1;
    }
    str_new[index] = '\0';
    return str_new;
}

char *my_str_n_concat(unsigned int n, ...)
{
    va_list list;
    char *str = NULL;
    char *temp = NULL;

    va_start(list, n);
    str = strdup("");
    if (str == NULL)
        return NULL;
    for (unsigned int i = 0; i < n; ++i) {
        temp = my_str_concat(str, va_arg(list, char *));
        free(str);
        if (temp == NULL)
            return NULL;
        str = temp;
    }
    va_end(list);
    return str;
}

char *my_str_join(const char *separator, const char **arr)
{
    char *temp = NULL;
    char *output = NULL;

    if (separator == NULL || arr == NULL)
        return NULL;
    if (arr[0] == 0)
        return strdup("");
    if (arr[1] == 0)
        return strdup(arr[0]);
    temp = my_str_join(separator, &(arr[1]));
    output = my_str_n_concat(3, arr[0], separator, temp);
    if (temp != NULL)
        free(temp);
    return output;
}
