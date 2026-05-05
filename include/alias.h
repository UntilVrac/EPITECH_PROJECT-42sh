/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Alias structure and function prototypes
*/

#ifndef ALIAS_H_
    #define ALIAS_H_

typedef struct alias_s {
    char *name;
    char *command;
    struct alias_s *next_alias;
} alias_t;

// alias_utils.c
char *get_alias(alias_t *list, char *name);
void add_alias(alias_t **list, char *name, char *command);
void delete_alias(alias_t **list, char *name);
void free_alias_list(alias_t *list);
// alias_builtins.c
int check_alias_builtin(char **arg, alias_t **alias_list);
char *build_alias_command(char *alias_val, char **old_arg);
char **check_alias_expansion(char **arg, void *array[], int *last_return);

#endif /* ALIAS_H_ */
