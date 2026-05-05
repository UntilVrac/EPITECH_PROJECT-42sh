/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Functions to manipulate the editor_t struct
*/

#include "editor.h"

void move_left(editor_t *editor, char **env)
{
    if (editor->cursor > 0)
        editor->cursor--;
}

void move_right(editor_t *editor, char **env)
{
    if (editor->cursor < editor->len)
        editor->cursor++;
}

void delete_char(editor_t *editor, char **env)
{
    if (editor->cursor == 0)
        return;
    memmove(editor->buffer + editor->cursor - 1, editor->buffer +
        editor->cursor, editor->len - editor->cursor);
    editor->cursor--;
    editor->len--;
    editor->buffer[editor->len] = '\0';
}

void insert_char(editor_t *editor, char c)
{
    if (editor->len + 1 >= editor->cap) {
        editor->cap *= 2;
        editor->buffer = realloc(editor->buffer, editor->cap);
    }
    memmove(editor->buffer + editor->cursor + 1, editor->buffer +
        editor->cursor, editor->len - editor->cursor);
    editor->buffer[editor->cursor] = c;
    editor->cursor++;
    editor->len++;
    editor->buffer[editor->len] = '\0';
}
