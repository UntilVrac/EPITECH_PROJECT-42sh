/*
** EPITECH PROJECT, 2025
** libmy.a library
** File description:
** this file countains string tools functions
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "lang.h"
#include "functions.h"

char *my_substring(const char *str, int start_pos, unsigned int n)
{
    int size = strlen(str);
    int end_pos = start_pos + n;
    char *new_string;

    if (start_pos >= size) {
        fprintf(stderr, "IndexError : string index out of range\n");
        return NULL;
    }
    if (end_pos > size)
        end_pos = size;
    new_string = calloc(n + 1, sizeof(char));
    if (new_string == NULL)
        return NULL;
    for (int i = start_pos; i < end_pos; ++i)
        new_string[i - start_pos] = str[i];
    new_string[end_pos - start_pos] = '\0';
    return new_string;
}

static int set_array(char **arr, const char *str, char sep, int size)
{
    int index = 0;
    int start = 0;

    for (int i = 0; i < size; ++i) {
        if (str[i] == sep) {
            arr[index] = my_substring(str, start, (i - start));
            start = i + 1;
            index += 1;
        }
    }
    if (start < size)
        arr[index] = my_substring(str, start, (size - start));
    else
        arr[index] = strdup("");
    if (arr[index] == NULL) {
        free_array(arr);
        return EPITECH_FAILURE;
    }
    arr[index + 1] = 0;
    return EPITECH_SUCCESS;
}

char **my_split_str(const char *str, char sep)
{
    const int size = strlen(str);
    int nbr_occ = my_nbr_occurences_in_str(sep, str);
    int verif = 0;
    char **arr = calloc(nbr_occ + 2, sizeof(char *));

    if (arr == NULL)
        return NULL;
    if (nbr_occ == 0) {
        arr[0] = strdup(str);
        if (!(arr[0])) {
            free(arr);
            return NULL;
        }
    } else {
        verif = set_array(arr, str, sep, size);
        if (verif != EPITECH_SUCCESS)
            return NULL;
    }
    return arr;
}
