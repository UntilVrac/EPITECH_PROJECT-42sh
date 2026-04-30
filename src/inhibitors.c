/*
** EPITECH PROJECT, 2026
** inhibitors.c
** File description:
** manage all the inhibitors of 42sh
*/

#include <stdbool.h>
#include "functions.h"
#include "lang.h"

static void find_nb_quote(const char **new_arg, size_t y, size_t *nb_quote)
{
    for (size_t x = 0; new_arg[y][x] != '\0'; x++)
        if (new_arg[y][x] == '\'' &&
            (x == 0 || (x != 0 && new_arg[y][x - 1] != '\\')))
            *nb_quote += 1;
}

char **check_quotes(char **new_arg, const char *separator)
{
    size_t nb_quote = 0;

    if (strcmp(separator, ";") != 0)
        return new_arg;
    for (size_t y = 0; new_arg[y] != NULL; y++)
        find_nb_quote((const char **)new_arg, y, &nb_quote);
    if (nb_quote % 2 != 0) {
        fprintf(stderr, "Unmatched '''.\n");
        for (size_t x = 0; new_arg[x] != NULL; x++)
            free(new_arg[x]);
        free(new_arg);
        return NULL;
    } else if (nb_quote != 0)
        new_arg = assemble_quote((const char **)new_arg, separator);
    return new_arg;
}
