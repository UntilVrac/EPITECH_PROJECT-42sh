/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Header file for editor
*/

#ifndef EDITOR_H_
    #define EDITOR_H_

    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <fcntl.h>
    #include "jobs.h"
    #include <ncurses.h>
    #include "functions.h"
    #include "lang.h"

    #define MAX_BINDINGS 256

typedef struct {
    char *buffer;
    int len;
    int cap;
    int cursor;
    int prompt_len;
    int prompt_row;
} editor_t;

typedef void (*key_fn_t)(editor_t *editor, char **env);

typedef struct {
    int key;
    key_fn_t fn;
} binding_t;

extern binding_t bindings[MAX_BINDINGS];
extern int b_count;

// multiline.c
void init_editor(void);
void cleanup_editor(void);
void refresh_display(editor_t *editor, const char *prompt);
char *read_line(const char *prompt, char **env);
// binding.c
void add_bindkey(int key, key_fn_t fn);
key_fn_t find_bindkey(int key);
void init_bindings(void);
// actions.c
void move_left(editor_t *editor, char **env);
void move_right(editor_t *editor, char **env);
void delete_char(editor_t *editor, char **env);
void insert_char(editor_t *editor, char c);

#endif
