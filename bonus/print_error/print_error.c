/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** this file countains all functions to print error message in the language
** according to the env
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include "lang.h"
#include "functions.h"

static bool is_lang_available(const char *lang_file)
{
    struct stat sb;

    return (stat(lang_file, &sb) != -1);
}

static char *get_lang_file(const char *lang, const char *path_to_lang)
{
    char *lang_file = NULL;
    bool lang_available;

    if (lang == NULL)
        lang = DEFAULT_LANG;
    lang_file = my_str_n_concat(3, path_to_lang, lang, ".lang");
    if (lang_file == NULL)
        return NULL;
    lang_available = is_lang_available((const char *)(lang_file));
    if (!lang_available) {
        free(lang_file);
        if (strcmp(lang, DEFAULT_LANG) == 0)
            return NULL;
        return get_lang_file(DEFAULT_LANG, path_to_lang);
    }
    return lang_file;
}

static char *language_error(const char *lang_file, err_msg_t err_msg,
    char **arr)
{
    free_array(arr);
    if (err_msg == DEFAULT)
        return NULL;
    return get_error_message(lang_file, DEFAULT);
}

char *get_error_message(const char *lang_file, err_msg_t err_msg)
{
    char **content = get_file_content((char *)(lang_file));
    char **splited = NULL;
    char *msg = NULL;

    if (content == NULL)
        return NULL;
    for (int i = 0; content[i] != 0; ++i) {
        splited = my_split_str(content[i], '=');
        if (splited == NULL)
            return language_error(lang_file, err_msg, content);
        if (strcmp(splited[0], ERROR_MESSAGES[err_msg]) == 0){
            msg = my_str_join("=", (const char **)(&(splited[1])));
            free_array(splited);
            free_array(content);
            return msg;
        }
        free_array(splited);
    }
    return language_error(lang_file, err_msg, content);
}

static void print_message(char *prefix, char *msg)
{
    if (prefix != NULL) {
        fprintf(stderr, "%s", prefix);
        fprintf(stderr, ": ");
    }
    fprintf(stderr, "%s\n", msg);
}

static void put_error(err_msg_t err_msg, const char *msg_data[2],
    char *path_to_lang)
{
    char *prefix = (char *)(msg_data[0]);
    const char *lang = msg_data[1];
    char *lang_file = NULL;
    char *msg = my_str_n_concat(3, path_to_lang, ENV_DIR_NAME, "/");

    free(path_to_lang);
    path_to_lang = msg;
    lang_file = get_lang_file(lang, path_to_lang);
    free(path_to_lang);
    if (lang_file == NULL)
        return print_message(prefix, DEFAULT_MESSAGES[err_msg]);
    msg = get_error_message((const char *)(lang_file), err_msg);
    free(lang_file);
    if (msg == NULL)
        return print_message(prefix, DEFAULT_MESSAGES[err_msg]);
    print_message(prefix, msg);
    free(msg);
}

char **get_line_from_env(const char **env, const char *name)
{
    char **splited_line = NULL;
    char **line = NULL;

    for (int i = 0; env[i] != NULL; ++i) {
        splited_line = my_split_str(env[i], '=');
        if (splited_line == NULL)
            return NULL;
        if (strcmp(splited_line[0], (char *)(name)) == 0)
            line = (char **)(&(env[i]));
        free_array(splited_line);
        if (line != NULL)
            return line;
    }
    return NULL;
}

void print_error(const char *prefix, err_msg_t err_msg, const char **env)
{
    char **env_line = get_line_from_env(env, LANG_ENV_VAR);
    char **splited = NULL;
    char **temp = NULL;
    char *msg[2] = {(char *)(prefix), NULL};
    char *env_dir_path = get_path_to_simlink(env);

    if (env_line == NULL)
        return put_error(err_msg, (const char **)(msg), env_dir_path);
    splited = my_split_str(*env_line, '=');
    if (splited == NULL)
        return put_error(err_msg, (const char **)(msg), env_dir_path);
    temp = my_split_str(splited[1], '_');
    free_array(splited);
    if (temp == NULL)
        return put_error(err_msg, (const char **)(msg), env_dir_path);
    msg[1] = strdup(temp[0]);
    free_array(temp);
    put_error(err_msg, (const char **)(msg), env_dir_path);
    if (msg[1] != NULL)
        free(msg[1]);
}
