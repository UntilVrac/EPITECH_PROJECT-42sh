/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Handle line reading and completion
*/

#include "editor.h"

void free_editor(editor_t *editor)
{
    if (!editor)
        return;
    if (editor->buffer)
        free(editor->buffer);
    free(editor);
}

static void reset_editor(editor_t *editor, const char *prompt)
{
    if (editor->buffer)
        memset(editor->buffer, 0, editor->cap);
    editor->len = 0;
    editor->cursor = 0;
    editor->prompt_len = strlen(prompt);
}

void init_editor(void)
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
}

void cleanup_editor(void)
{
    endwin();
}

void refresh_display(editor_t *editor, const char *prompt)
{
    int rows;
    int cols;
    int abs_cursor_pos = editor->prompt_len + editor->cursor;
    int cursor_row;
    int cursor_col;
    int end_row;
    int total_len = editor->prompt_len + editor->len;

    getmaxyx(stdscr, rows, cols);
    end_row = editor->prompt_row + (total_len / cols);
    if (end_row >= rows)
        editor->prompt_row -= (end_row - rows + 1);
    move(editor->prompt_row, 0);
    clrtobot();
    printw("%s%s", prompt, editor->buffer);
    cursor_row = editor->prompt_row + (abs_cursor_pos / cols);
    cursor_col = abs_cursor_pos % cols;
    move(cursor_row, cursor_col);
    refresh();
}

static void handle_keypress(editor_t *editor, int ch, char **env)
{
    key_fn_t action = find_bindkey(editor, ch);

    if (action) {
        action(editor, env);
        return;
    }
    if (ch >= 32 && ch <= 126)
        insert_char(editor, ch);
}

char *read_line(const char *prompt, char **env, editor_t *editor)
{
    int ch;
    int col;

    reset_editor(editor, prompt);
    init_editor();
    getyx(stdscr, editor->prompt_row, col);
    refresh_display(editor, prompt);
    ch = getch();
    while (ch != '\n' && ch != 4) {
        handle_keypress(editor, ch, env);
        refresh_display(editor, prompt);
        ch = getch();
    }
    cleanup_editor();
    if (ch == 4 && editor->len == 0)
        return NULL;
    return strdup(editor->buffer);
}
