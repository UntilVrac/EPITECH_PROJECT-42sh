/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Handle line reading and completion
*/

#include "editor.h"

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
    int term_rows;
    int term_cols;
    int abs_cursor_pos;
    int cursor_row;
    int cursor_col;
    int total_len;

    getmaxyx(stdscr, term_rows, term_cols);
    move(editor->prompt_row, 0);
    clrtobot();
    printw("%s%s", prompt, editor->buffer);
    abs_cursor_pos = editor->prompt_len + editor->cursor;
    cursor_row = editor->prompt_row + (abs_cursor_pos / term_cols);
    cursor_col = abs_cursor_pos % term_cols;
    move(cursor_row, cursor_col);
    refresh();
}
