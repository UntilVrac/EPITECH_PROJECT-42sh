/*
** EPITECH PROJECT, 2026
** history_management.c
** File description:
** manage the history command and the associated flags
*/

#include "functions.h"
#include "history.h"
#include "lang.h"

static int history_len(history_t **history)
{
    int nb = 0;

    if (!history)
        return nb;
    while (history[nb] != NULL)
        nb++;
    return nb;
}

static void display_history(history_t **history, int nb)
{
    int start_value = history_len(history) - nb;

    for (int x = 0; history[x] != NULL; x++)
        if (x >= start_value)
            printf("\t%zu\t%s\t%s", history[x]->nb, history[x]->time,
                history[x]->command);
}

static void display_flag_history(history_t **history, const char **arg)
{
    char *buff = NULL;
    int nb = strtol(arg[1], &buff, 10);

    if (buff && buff[0] == '\0')
        display_history(history, nb);
    else
        fprintf(stderr, "history: Badly formed number.\n");
}

void history_management(const char **arg, history_t **history)
{
    if (my_word_array_len(arg) > 3)
        fprintf(stderr, "history: Too many arguments.\n");
    if (my_word_array_len(arg) == 1)
        display_history(history, history_len(history));
    if (my_word_array_len(arg) == 2 || my_word_array_len(arg) == 3)
        display_flag_history(history, arg);
}
