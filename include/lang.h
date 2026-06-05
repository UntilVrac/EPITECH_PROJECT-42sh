/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** this header file countains all error messages for several languages
*/

#ifndef LANG_
    #define LANG_

    #include <stddef.h>
    #include <string.h>

    #define EPITECH_SUCCESS 0
    #define EPITECH_FAILURE 84

    #define LANG_ENV_VAR "LANGUAGE"
    #define DEFAULT_LANG "en"

typedef char *msg_name_t;

static const msg_name_t DEFAULT_MESSAGES[] = {
    "Command not found.",
    "Expression syntax.",
    "Too many arguments.",
    "Too few arguments.",
    "No such file or directory.",
    "No such file.",
    "Invalid null command.",
    "Segmentation fault",
    "Segmentation fault (core dumped)",
    "Floating exception",
    "Floating exception (core dumped)",
    "No $home variable set.",
    "Not a directory.",
    "Permission denied.",
    "Variable name must begin with a letter.",
    "Variable name must contain alphanumeric characters.",
    "Exec format error. Binary file not executable.",
    "Ambiguous input redirect.",
    "Ambiguous output redirect.",
    "Missing name for redirect.",
    "Invalid argument.",
    "Error"
};

static const msg_name_t ERROR_MESSAGES[] = {
    "CMD_NOT_FOUND",
    "EXP_SYNTAX",
    "TOO_M_ARGS",
    "TOO_F_ARGS",
    "NO_SUCH_FILE_OR_DIR",
    "NO_SUCH_FILE",
    "NULL_CMD",
    "SEG_FAULT",
    "SEG_FAULT_CD",
    "FLOAT_POINT_EXCEPT",
    "FLOAT_POINT_EXCEPT_CD",
    "NO_HOME_DIR",
    "NOT_A_DIR",
    "PERM_DENIED",
    "VAR_NAME_M_BEG_W_LETTER",
    "VAR_NAME_M_CON_ALPHNUM_CHARS",
    "NOT_EXEC",
    "AMB_INPUT_RED",
    "AMB_OUTPUT_RED",
    "MISS_NAME_FOR_RED",
    "INVALID_ARG",
    "DEFAULT",
    NULL};

enum err_msg_e {
    CMD_NOT_FOUND,
    EXP_SYNTAX,
    TOO_M_ARGS,
    TOO_F_ARGS,
    NO_SUCH_FILE_OR_DIR,
    NO_SUCH_FILE,
    NULL_CMD,
    SEG_FAULT,
    SEG_FAULT_CD,
    FLOAT_POINT_EXCEPT,
    FLOAT_POINT_EXCEPT_CD,
    NO_HOME_DIR,
    NOT_A_DIR,
    PERM_DENIED,
    VAR_NAME_M_BEG_W_LETTER,
    VAR_NAME_M_CON_ALPHNUM_CHARS,
    NOT_EXEC,
    AMB_INPUT_RED,
    AMB_OUTPUT_RED,
    MISS_NAME_FOR_RED,
    INVALID_ARG,
    DEFAULT
} typedef err_msg_t;

    #define ENV_DIR_NAME "42sh-env"
    #define ENV_DIR_PATH "/42sh-env"

    #define LN "/bin/ln"
    #define HOME "/home/"
    #define LEN_HOME (strlen(HOME))
    #define TMP_FILE "tmp_file_env"

void print_error(const char *prefix, err_msg_t err_msg, const char **env);
char *get_error_message(const char *lang_file, err_msg_t err_msg);
// char *get_path_to_env_dir(void);
void init_env_dir(const char **env);
char **get_line_from_env(const char **env, const char *name);
char *get_path_to_simlink(const char **env);

char *my_str_concat(const char *str1, const char *str2);
char *my_str_n_concat(unsigned int n, ...);
int my_nbr_occurences_in_str(char c, const char *str);
char *my_substring(const char *str, int start_pos, unsigned int n);
char **my_split_str(const char *str, char sep);
void my_replace_in_str(char *str, char c, char c_new);
char **get_file_content(char *filename);
char *my_str_join(const char *separator, const char **arr);
int my_word_array_len(const char **arr);

#endif /* LANG_ */
