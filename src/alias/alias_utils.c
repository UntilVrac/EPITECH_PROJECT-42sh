/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Alias helper functions
*/

#include "functions.h"
#include "lang.h"
#include "jobs.h"
#include "alias.h"

char *get_alias(alias_t *list, char *name)
{
    if (!name)
        return NULL;
    for (alias_t *tmp = list; tmp != NULL; tmp = tmp->next_alias) {
        if (strcmp(tmp->name, name) == 0)
            return tmp->command;
    }
    return NULL;
}

void add_alias(alias_t **list, char *name, char *command)
{
    alias_t *new_alias = NULL;

    for (alias_t *tmp = *list; tmp != NULL; tmp = tmp->next_alias) {
        if (strcmp(tmp->name, name) == 0) {
            free(tmp->command);
            tmp->command = strdup(command);
            return;
        }
    }
    new_alias = malloc(sizeof(alias_t));
    if (!new_alias)
        return;
    new_alias->name = strdup(name);
    new_alias->command = strdup(command);
    new_alias->next_alias = *list;
    *list = new_alias;
}

static void remove_alias_node(alias_t **list, alias_t *tmp, alias_t *prev)
{
    if (prev == NULL)
        *list = tmp->next_alias;
    else
        prev->next_alias = tmp->next_alias;
    free(tmp->name);
    free(tmp->command);
    free(tmp);
}

void delete_alias(alias_t **list, char *name)
{
    alias_t *prev = NULL;

    if (name == NULL || *list == NULL)
        return;
    for (alias_t *tmp = *list; tmp != NULL; tmp = tmp->next_alias) {
        if (strcmp(tmp->name, name) == 0) {
            remove_alias_node(list, tmp, prev);
            return;
        }
        prev = tmp;
    }
}

void free_alias_list(alias_t *list)
{
    alias_t *tmp = NULL;

    while (list) {
        tmp = list;
        list = list->next_alias;
        free(tmp->name);
        free(tmp->command);
        free(tmp);
    }
}
