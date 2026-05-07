/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Minishell 2 project
*/

#include <string.h>
#include "functions.h"
#include "lang.h"
#include "alias.h"
#include "history.h"
#include "editor.h"

static char **get_env_copy(char **env)
{
    int i = 0;
    char **copy_env = NULL;

    while (env[i])
        i++;
    copy_env = malloc(sizeof(char *) * (i + 1));
    if (!copy_env) {
        perror("malloc copy_env");
        exit(84);
    }
    for (int j = 0; j < i; j++) {
        copy_env[j] = strdup((const char *)(env[j]));
    }
    copy_env[i] = NULL;
    return copy_env;
}

static void **update_structs(alias_t **alias, history_t **history,
    jobs_t **jobs, editor_t *editor)
{
    void **structs = malloc(sizeof(void *) * 5);

    if (!structs)
        return NULL;
    structs[0] = alias;
    structs[1] = history;
    structs[2] = jobs;
    structs[3] = editor;
    structs[4] = NULL;
    return structs;
}

char **process_line(void *data[], char **copy_env, int *last_return)
{
    void **structs = update_structs(data[1], data[2], data[3], data[4]);
    char **commands = NULL;
    char *line = (char *)data[0];
    int len = strlen((const char *)(line));
    jobs_t **jobs = (jobs_t **)data[3];

    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    line = handle_backticks(line, last_return, structs, copy_env);
    commands = split_semicolon(line);
    if (line)
        free(line);
    if (!commands)
        return copy_env;
    for (int i = 0; commands[i] != NULL; i++)
        copy_env = parse_command(commands[i], (void *[]) {copy_env, data[1],
                commands, jobs, data[2], data[4]}, last_return, jobs);
    free_array(commands);
    free(structs);
    return copy_env;
}

void print_exit(void)
{
    if (isatty(STDIN_FILENO))
        printf("exit\n");
}

static void clean_exit(jobs_t *jobs, char *line, alias_t *alias_list)
{
    print_exit();
    free_alias_list(alias_list);
    free_jobs_struct(jobs);
    free(line);
}

static editor_t *init_ed_struct(const char *prompt)
{
    editor_t *editor = malloc(sizeof(editor_t));

    if (!editor)
        return NULL;
    editor->buffer = calloc(256, 1);
    editor->len = 0;
    editor->cap = 256;
    editor->cursor = 0;
    editor->prompt_len = strlen(prompt);
    editor->b_count = 0;
    if (!editor->buffer)
        return NULL;
    return editor;
}

static void execute_line(char *line, void *data[], int *last_return)
{
    char **copy_env = (char **)data[0];
    history_t **history = (history_t **)data[2];
    size_t *current = (size_t *)data[5];

    history = add_command_to_history(history, line, current);
    data[0] = process_line(data, copy_env, last_return);
}

static void read_input(char ***copy_env, int *last_return)
{
    char *line = NULL;
    jobs_t *jobs = init_jobs_struct();
    alias_t *alias_list = NULL;
    history_t **history = NULL;
    editor_t *editor = init_ed_struct("");
    size_t current = 0;

    init_bindings(editor);
    while (1) {
        display_custom_prompt(*copy_env);
        line = read_line("", *copy_env, editor);
        if (!line)
            break;
        execute_line(line, (void *[]){*copy_env, &alias_list,
                &history, jobs, editor, &current}, last_return);
    }
    free_editor(editor);
    clean_exit(jobs, line, alias_list);
}

int main(int argc, char **argv, char **env)
{
    char **copy_env = get_env_copy(env);
    int last_return = 0;

    init_env_dir((const char **)(copy_env));
    (void)argc;
    (void)argv;
    for (size_t i = 0; ignored_signals[i] != 0; ++i)
        signal(ignored_signals[i], SIG_IGN);
    read_input(&copy_env, &last_return);
    free_array(copy_env);
    return last_return;
}
