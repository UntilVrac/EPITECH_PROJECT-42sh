/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Dynamic rebinding functions
*/

#include "editor.h"

binding_t bindings[MAX_BINDINGS];
int b_count = 0;

void add_bindkey(int key, key_fn_t fn)
{
    for (int i = 0; i < b_count; i++)
        if (bindings[i].key == key) {
            bindings[i].fn = fn;
            return;
        }
    if (b_count < MAX_BINDINGS) {
        bindings[b_count] = (binding_t){key, fn};
        b_count++;
    }
}

key_fn_t find_bindkey(int key)
{
    for (int i = 0; i < b_count; i++)
        if (bindings[i].key == key)
            return bindings[i].fn;
    return NULL;
}

void init_bindings(void)
{
    add_bindkey(KEY_LEFT, move_left);
    add_bindkey(KEY_RIGHT, move_right);
    add_bindkey(KEY_BACKSPACE, delete_char);
    add_bindkey(127, delete_char);
}
