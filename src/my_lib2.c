/*
** EPITECH PROJECT, 2026
** MINISHELL 2
** File description:
** Library functions
*/

#include "../include/functions.h"

char *my_strcat(char *dest, char *src)
{
    int i = my_strlen(dest);
    int j = 0;

    while (src[j] != '\0') {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
    return dest;
}

char *my_strchr(char *str, char c)
{
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == c)
            return &str[i];
        i++;
    }
    return NULL;
}
