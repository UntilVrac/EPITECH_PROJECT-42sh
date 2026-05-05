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

typedef struct {
    char *buffer;
    int len;
    int cap;
    int cursor;
    int prompt_len;
    int prompt_row;
} editor_t;


void init_editor(void);
void cleanup_editor(void);
void refresh_display(editor_t *editor, const char *prompt);

#endif
