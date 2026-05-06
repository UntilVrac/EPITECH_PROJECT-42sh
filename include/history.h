/*
** EPITECH PROJECT, 2026
** history.h
** File description:
** header for the history command
*/

#ifndef HISTORY_H_
    #define HISTORY_H_

typedef struct history {
    size_t nb;
    char *time;
    char *command;
} history_t;

history_t **add_command_to_history(history_t **history, char *new_command,
    size_t *current);
void history_management(const char **arg, history_t **history);
void free_history_struct(history_t **history);

#endif /* HISTORY_H_ */
