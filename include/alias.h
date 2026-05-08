/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Alias structure and function prototypes
*/

#ifndef ALIAS_H_
    #define ALIAS_H_

    #include "history.h"

typedef struct alias_s {
    char *name;
    char *command;
    struct alias_s *next_alias;
} alias_t;

// string_arr_tools.c
bool my_is_in_str_array(const char *str, const char **arr);
bool cpy_string_arr(const char **src, char **dest,
    bool if_not_in_arr, const char **old_cmd);
void my_show_string_arr(const char **arr);

// alias_utils.c
char *get_alias(alias_t *list, char *name);
void add_alias(alias_t **list, char *name, char *command);
void delete_alias(alias_t **list, char *name);
void free_alias_list(alias_t *list);
// alias_builtins.c
int check_alias_builtin(char **arg, alias_t **alias_list);
char *build_alias_command(const char *alias_val, const char **old_arg);
char **check_alias_expansion(char **arg, void *array[], int *last_return);

#endif /* ALIAS_H_ */
