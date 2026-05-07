/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Dynamic rebinding functions
*/

#include "editor.h"

char *get_func_by_name(key_fn_t fn)
{
    const map_t *table = get_action_table();

    for (int i = 0; table[i].fn; i++)
        if (table[i].fn == fn)
            return table[i].name;
    return "Unknown action";
}

int parse_key(char *key)
{
    if (strlen(key) == 2 && key[0] == '^')
        return key[1] - 'A' + 1;
    if (strcmp(key, "LEFT") == 0)
        return KEY_LEFT;
    if (strcmp(key, "RIGHT") == 0)
        return KEY_RIGHT;
    if (strcmp(key, "BACKSPACE") == 0)
        return KEY_BACKSPACE;
    if (strlen(key) == 1)
        return key[0];
    return -1;
}
