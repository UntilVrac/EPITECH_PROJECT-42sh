/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** this file countains all functions to get the env dir countaining .lang files
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include "lang.h"
#include "functions.h"

// char *get_path_to_env_dir(void)
// {
//     char *cwd = getcwd(NULL, 0);
//
//     if (cwd == NULL)
//         return NULL;
//     return my_str_n_concat(4, cwd, "/", ENV_DIR_NAME, "/");
// }
// static bool check_cwd(const char *cwd)
// {
//     char *tmp = my_substring(cwd, 0, LEN_HOME);
//     char *user_name = NULL;
//     char **splited = NULL;
//
//     if (tmp == NULL)
//         return false;
//     if (strcmp((const char *)(tmp), HOME) != 0) {
//         free(tmp);
//         return false;
//     }
//     free(tmp);
//     splited = my_split_str((const char *)(&(cwd[LEN_HOME])), '/');
//     if (splited == NULL)
//         return false;
//     if (splited[0] == 0) {
//         free(splited);
//         return false;
//     }
//     return true;
// }
//
// static char *get_path_to_simlink(const char *cwd)
// {
//     char *tmp = my_substring(cwd, 0, LEN_HOME);
//     char *user_name = NULL;
//     char **splited = NULL;
//     char *path_to_simlink = NULL;
//
//     if (!check_cwd(cwd))
//         return my_str_n_concat(3, cwd, "/", ENV_DIR_NAME);
//     tmp = my_substring(cwd, 0, LEN_HOME + strlen(splited[0]) + 1);
//     free_array(splited);
//     path_to_simlink = my_str_concat(tmp, ENV_DIR_NAME);
//     free(tmp);
//     return path_to_simlink;
// }
//
char *get_path_to_simlink(const char **env)
{
    char **line = get_line_from_env(env, "HOME");
    char **splited = NULL;
    char *path = NULL;

    if (line == NULL)
        return ENV_DIR_PATH;
    if (*line == NULL)
        return ENV_DIR_PATH;
    splited = my_split_str((const char *)(*line), '=');
    if (splited == NULL)
        return ENV_DIR_PATH;
    if (my_word_array_len((const char **)(splited)) < 2) {
        free_array(splited);
        return ENV_DIR_PATH;
    }
    path = my_str_n_concat(3, splited[1], "/", ENV_DIR_NAME);
    free_array(splited);
    return path;
}

static void free_paths(char *path1, char *path2)
{
    if (path1 != NULL)
        free(path1);
    if (path2 != NULL)
        free(path2);
}

static void create_simlink(const char *path_to_env_dir,
    const char *path_to_link, const char **env)
{
    int fd = open(TMP_FILE, O_WRONLY | O_CREAT, 666);

    if (fd > 0) {
        // dup2(fd, 1);
        // dup2(fd, 2);
    }
    printf("execve\n");
    execve(LN, (char *const[5]){"ln", "-sf", (char *const)(path_to_env_dir),
            (char *const)(path_to_link), NULL}, (char *const *)(env));
    close(fd);
    free_paths((char *)(path_to_link), (char *)(path_to_env_dir));
    _exit(EPITECH_FAILURE);
}

void init_env_dir(const char **env)
{
    char *cwd = getcwd(NULL, 0);
    char *path_to_env_dir = NULL;
    char *path_to_link = get_path_to_simlink(env);
    int pid = 0;
    int stat = 0;

    if (cwd == NULL || path_to_link == NULL)
        return free_paths(cwd, path_to_link);
    path_to_env_dir = my_str_n_concat(4, cwd, "/", ENV_DIR_NAME, "/");
    free(cwd);
    if (path_to_env_dir == NULL)
        return free_paths(path_to_link, NULL);
    pid = fork();
    if (pid == 0)
        create_simlink((const char *)(path_to_env_dir),
            (const char *)(path_to_link), env);
    waitpid(pid, &stat, 0);
    remove(TMP_FILE);
    free_paths(path_to_link, path_to_env_dir);
}

// void init_env_dir(const char **env)
// {
//     char *cwd = getcwd(NULL, 0);
//     char *path_to_env_dir = NULL;
//     char *path_to_link = NULL;
//     // char **path_home = get_line_from_env(env, "HOME");
//     int pid = 0;
//
//     if (cwd == NULL)
//         return;
//     path_to_env_dir = my_str_n_concat(4, cwd, "/", ENV_DIR_NAME, "/");
//     path_to_link = get_path_to_simlink((const char *)(cwd));
//     printf("cwd : '%s'\n", cwd);
//     free(cwd);
//     pid = fork();
//     if (pid == 0) {
//         execve(LN, (char *const[]){"ln", "-sf", path_to_env_dir,
//                 ENV_DIR_PATH, NULL}, (char *const *)(env));
//         _exit(EPITECH_FAILURE);
//     }
//     free(path_to_env_dir);
// }
