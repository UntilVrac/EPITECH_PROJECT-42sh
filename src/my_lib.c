/*
** EPITECH PROJECT, 2026
** MINISHELL 2
** File description:
** Library functions
*/

#include "../include/functions.h"

int my_strlen(char *s)
{
    int length = 0;

    while (s[length] != '\0') {
        length++;
    }
    return length;
}

char *my_strdup(char *src)
{
    int length = my_strlen(src);
    char *dest = malloc(sizeof(char) * (length + 1));
    int i = 0;

    if (!dest) {
        perror("malloc error strdup");
        exit(84);
    }
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

int my_strcmp(char *str1, char *str2)
{
    int i = 0;

    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i])
            return 1;
        i++;
    }
    if (str1[i] == '\0' && str2[i] == '\0')
        return 0;
    return 1;
}

int my_strncmp(char *str1, char *str2, int n)
{
    int i = 0;

    while (i < n && str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i])
            return 1;
        i++;
    }
    if (n == i)
        return 0;
    return 1;
}
