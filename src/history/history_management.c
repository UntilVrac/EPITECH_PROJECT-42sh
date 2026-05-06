/*
** EPITECH PROJECT, 2026
** history_management.c
** File description:
** manage the history command and the associated flags
*/

#include "functions.h"
#include "history.h"
#include "lang.h"

static void display_history(history_t **history)
{
    for (size_t x = 0; history[x] != NULL; x++)
        printf("\t%zu\t%s\t%s", history[x]->nb, history[x]->time,
            history[x]->command);
}

void history_management(const char **arg, history_t **history)
{
    if (my_word_array_len(arg) == 1)
        display_history(history);
}
