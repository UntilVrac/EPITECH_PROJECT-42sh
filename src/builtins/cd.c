/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** Functions for moving through directories
*/

#include <string.h>
#include "lang.h"
#include "functions.h"

static char *get_cd_path(char **arg, char **copy_env)
{
    char *path = arg[1];
    int index = 0;

    if (!path || strcmp((const char *)(path), "~") == 0) {
        index = get_env_var_index((const char **)(copy_env), "HOME");
        if (index == -1) {
            print_error("cd", NO_HOME_DIR, (const char **)(copy_env));
            return NULL;
        }
        return &copy_env[index][5];
    }
    if (strcmp((const char *)(path), "-") == 0) {
        index = get_env_var_index((const char **)(copy_env), "OLDPWD");
        if (index == -1) {
            print_error("", NO_SUCH_FILE_OR_DIR, (const char **)(copy_env));
            return NULL;
        }
        return &copy_env[index][7];
    }
    return path;
}

static char **update_oldpwd(char **copy_env, char *old_cwd, void **structs)
{
    int ignore_return = 0;
    char *arg[4] = {"setenv", "OLDPWD", old_cwd, NULL};

    (void)structs;
    return execute_setenv(arg, copy_env, &ignore_return, structs);
}

static char **update_pwd(char **copy_env, void **structs)
{
    int ignore_return = 0;
    char cwd[1024];
    char *arg[4] = {"setenv", "PWD", NULL, NULL};

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        arg[2] = cwd;
        copy_env = execute_setenv(arg, copy_env, &ignore_return, structs);
    }
    return copy_env;
}

static int check_if_directory(char *path, const char **env)
{
    struct stat st;

    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode) == 0) {
        print_error(path, NOT_A_DIR, env);
        return -1;
    }
    return 0;
}

static void display_correct_message(char *path, const char **env)
{
    if (access(path, F_OK) == -1)
        print_error((const char *)(path), NO_SUCH_FILE_OR_DIR, env);
    else if (access(path, X_OK) == -1)
        print_error((const char *)(path), PERM_DENIED, env);
}

char **execute_cd(char **arg, char **copy_env, int *last_return, void **structs)
{
    char *path = get_cd_path(arg, copy_env);
    char old_cwd[1024];

    if (!path || check_if_directory(path, (const char **)(copy_env)) == -1) {
        *last_return = 1;
        return copy_env;
    }
    if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
        return copy_env;
    if (chdir(path) == -1) {
        display_correct_message(path, (const char **)(copy_env));
        *last_return = 1;
    } else {
        copy_env = update_oldpwd(copy_env, old_cwd, structs);
        copy_env = update_pwd(copy_env, structs);
        *last_return = 0;
    }
    return copy_env;
}
