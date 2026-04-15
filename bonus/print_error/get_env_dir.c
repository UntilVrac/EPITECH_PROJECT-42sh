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

char *get_path_to_simlink(const char **env)
{
    char **line = get_line_from_env(env, "HOME");
    char **splited = NULL;
    char *path = NULL;

    if (line == NULL)
        return strdup("/");
    if (*line == NULL)
        return strdup("/");
    splited = my_split_str((const char *)(*line), '=');
    if (splited == NULL)
        return strdup("/");
    if (my_word_array_len((const char **)(splited)) < 2) {
        free_array(splited);
        return strdup("/");
    }
    path = my_str_concat(splited[1], "/");
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
        dup2(fd, 1);
        dup2(fd, 2);
    }
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
