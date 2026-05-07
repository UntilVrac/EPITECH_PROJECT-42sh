/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Dynamic rebinding functions
*/

#include "editor.h"

const map_t *get_action_table(void)
{
    static const map_t action_table[] = {
        {"move_left", &move_left},
        {"move_right", &move_right},
        {"delete_char", &delete_char},
        {NULL, NULL}
    };

    return action_table;
}

void add_bindkey(editor_t *editor, int key, key_fn_t fn)
{
    for (int i = 0; i < editor->b_count; i++)
        if (editor->bindings[i].key == key) {
            editor->bindings[i].fn = fn;
            return;
        }
    if (editor->b_count < MAX_BINDINGS) {
        editor->bindings[editor->b_count] = (binding_t){key, fn};
        editor->b_count++;
    }
}

key_fn_t find_bindkey(editor_t *editor, int key)
{
    for (int i = 0; i < editor->b_count; i++)
        if (editor->bindings[i].key == key)
            return editor->bindings[i].fn;
    return NULL;
}

void init_bindings(editor_t *editor)
{
    add_bindkey(editor, KEY_LEFT, move_left);
    add_bindkey(editor, KEY_RIGHT, move_right);
    add_bindkey(editor, KEY_BACKSPACE, delete_char);
    add_bindkey(editor, 127, delete_char);
}

static void print_key_name(int key)
{
    if (key < 32) {
        printf("\"^%c\"", key + 64);
        return;
    }
    switch (key) {
        case KEY_LEFT:
            printf("left");
            break;
        case KEY_RIGHT:
            printf("right");
            break;
        case KEY_BACKSPACE:
            printf("\"^?\"");
            break;
        default:
            printf("\"%c\"", key);
    }
}

void display_bindings(editor_t *editor)
{
    int key;
    char *fn_name;

    printf("Standard key bindings\n");
    for (int i = 0; i < editor->b_count; i++) {
        key = editor->bindings[i].key;
        fn_name = get_func_by_name(editor->bindings[i].fn);
        print_key_name(key);
        printf("\t\t-> %s\n", fn_name);
    }
}
