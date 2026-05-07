/*
** EPITECH PROJECT, 2026
** history.c
** File description:
** manage the history command
*/

#include <time.h>
#include "functions.h"
#include "history.h"

static char *search_command_in_history(size_t nb, history_t **history)
{
    char *command = NULL;

    for (size_t x = 0; history[x] != NULL; x++)
        if (nb == history[x]->nb)
            command = strdup(history[x]->command);
    return command;
}

static char **exclamation_mark_management(char **commands, size_t index,
    size_t index2, history_t **history)
{
    char *buff = NULL;
    size_t nb = strtol(&commands[index][index2 + 1], &buff, 10);
    char *new_command = NULL;

    if (strlen(commands[index]) == 1)
        return commands;
    else if (buff[0] != '\0') {
        fprintf(stderr, "%s: Event not found.\n", buff);
        free_array(commands);
        return NULL;
    } else
        new_command = search_command_in_history(nb, history);
    if (!new_command) {
        fprintf(stderr, "%zu: Event not found.\n", nb);
        free_array(commands);
        return NULL;
    }
    free(commands[index]);
    commands[index] = new_command;
    return commands;
}

char **check_exclamation_mark(history_t **history, char **commands)
{
    bool is_exc_mark_in_com = false;
    size_t nb = 0;

    if (!commands)
        return NULL;
    for (size_t x = 0; commands[x] != NULL; x++) {
        for (; commands[x][nb] == ' ' || commands[x][nb] == '\t'; nb++);
        if (commands[x][nb] == '!') {
            is_exc_mark_in_com = true;
            commands = exclamation_mark_management(commands, x, nb, history);
        }
        nb = 0;
        if (!commands)
            break;
    }
    if (commands && is_exc_mark_in_com == true)
        for (size_t x = 0; commands[x] != NULL; x++)
            printf("%s", commands[x]);
    return commands;
}

void free_history_struct(history_t **history)
{
    for (size_t x = 0; history[x] != NULL; x++) {
        free(history[x]->time);
        free(history[x]->command);
        free(history[x]);
    }
    free(history);
}

static char *get_current_time(void)
{
    time_t date = time(NULL);
    char *tmp = NULL;
    char result[6];
    size_t current = 0;

    tmp = strtok(ctime(&date), " ");
    while (strlen(tmp) != 8)
        tmp = strtok(0, " ");
    for (size_t x = 0; x != 5; x++) {
        if (x == 0 && tmp[x] == '0')
            x++;
        result[current] = tmp[x];
        current++;
    }
    result[current] = '\0';
    return strdup(result);
}

static history_t **add_first_command(char *new_command, size_t *current)
{
    history_t **history = malloc(sizeof(history_t *) * 2);
    history_t *part_history = malloc(sizeof(history_t));

    part_history->nb = 1;
    part_history->time = get_current_time();
    part_history->command = strdup(new_command);
    history[*current] = part_history;
    history[*current + 1] = NULL;
    *current += 1;
    return history;
}

static history_t **add_command(history_t **history, char *new_command,
    size_t *current)
{
    history_t *part_history = malloc(sizeof(history_t));

    *current += 1;
    part_history->nb = *current;
    part_history->time = get_current_time();
    part_history->command = strdup(new_command);
    history = realloc(history, sizeof(history_t *) * (*current + 1));
    history[*current - 1] = part_history;
    history[*current] = NULL;
    return history;
}

history_t **add_command_to_history(history_t **history, char *new_command,
    size_t *current)
{
    if (!history)
        return add_first_command(new_command, current);
    else
        history = add_command(history, new_command, current);
    return history;
}
