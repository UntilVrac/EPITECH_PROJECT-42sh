/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Bindkey function
*/

#include "editor.h"

static void update_binding(editor_t *editor, char *key_str, char *fn_name,
    int *last_return)
{
    int key = parse_key(key_str);
    key_fn_t fn = NULL;
    const map_t *table = get_action_table();

    for (int i = 0; table[i].fn; i++)
        if (strcmp(table[i].name, fn_name) == 0)
            fn = table[i].fn;
    if (key != -1 && fn != NULL) {
        add_bindkey(editor, key, fn);
        *last_return = 0;
    } else {
        *last_return = 1;
    }
}

char **execute_bindkey(char **args, char **env, int *last_return,
    void **structs)
{
    editor_t *editor = (editor_t *)structs[3];

    if (!editor || !args[1]) {
        if (editor) {
            display_bindings(editor);
            *last_return = 0;
        } else {
            *last_return = 1;
        }
        return env;
    }
    if (!args[2]) {
        *last_return = 1;
        return env;
    }
    update_binding(editor, args[1], args[2], last_return);
    return env;
}
