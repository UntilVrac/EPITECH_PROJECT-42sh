/*
** EPITECH PROJECT, 2026
** MINISHELL 2
** File description:
** Functions for moving through directories
*/

#include "../include/functions.h"

static char *get_cd_path(char **arg, char **copy_env)
{
    char *path = arg[1];
    int index = 0;

    if (!path || my_strcmp(path, "~") == 0) {
        index = get_env_var_index(copy_env, "HOME");
        if (index == -1) {
            write(2, "cd: No home directory.\n", 23);
            return NULL;
        }
        return &copy_env[index][5];
    }
    if (my_strcmp(path, "-") == 0) {
        index = get_env_var_index(copy_env, "OLDPWD");
        if (index == -1) {
            write(2, ": No such file or directory.\n", 29);
            return NULL;
        }
        return &copy_env[index][7];
    }
    return path;
}

static char **update_oldpwd(char **copy_env, char *old_cwd)
{
    int ignore_return = 0;
    char *arg[4] = {"setenv", "OLDPWD", old_cwd, NULL};

    return execute_setenv(arg, copy_env, &ignore_return);
}

static char **update_pwd(char **copy_env)
{
    int ignore_return = 0;
    char cwd[1024];
    char *arg[4] = {"setenv", "PWD", NULL, NULL};

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        arg[2] = cwd;
        copy_env = execute_setenv(arg, copy_env, &ignore_return);
    }
    return copy_env;
}

static int check_if_directory(char *path)
{
    struct stat st;
    const char *error = ": Not a directory.\n";

    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode) == 0) {
        write(2, path, my_strlen(path));
        write(2, error, my_strlen((char *)error));
        return -1;
    }
    return 0;
}

static void display_correct_message(char *path)
{
    const char *error1 = ": No such file or directory.\n";
    const char *error2 = ": Permission denied.\n";

    write(2, path, my_strlen(path));
    if (access(path, F_OK) == -1)
        write(2, error1, my_strlen((char *)error1));
    else if (access(path, X_OK) == -1)
        write(2, error2, my_strlen((char *)error2));
}

char **execute_cd(char **arg, char **copy_env, int *last_return)
{
    char *path = get_cd_path(arg, copy_env);
    char old_cwd[1024];

    if (!path || check_if_directory(path) == -1) {
        *last_return = 1;
        return copy_env;
    }
    if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
        return copy_env;
    if (chdir(path) == -1) {
        display_correct_message(path);
        *last_return = 1;
    } else {
        copy_env = update_oldpwd(copy_env, old_cwd);
        copy_env = update_pwd(copy_env);
        *last_return = 0;
    }
    return copy_env;
}
