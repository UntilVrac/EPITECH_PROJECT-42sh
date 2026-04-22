/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** jobs.h
*/

#ifndef JOBS_H_
    #define JOBS_H_

    #include <sys/types.h>
    #define JOBS_CMD "jobs"
    #define JOBS_BG_CMD "bg"
    #define JOBS_FG_CMD "fg"

typedef enum jobs_state {
    STOPPED,
    BACKGROUND,
    FOREGROUND,
    DONE,
    NULL_STATE,
    EXITED
} states_t;

typedef struct jobs_control {
    size_t pos;
    pid_t pid;
    states_t state;
    char *name;
} jobs_t;

typedef struct jobs_builtins_s {
    char *name;
    void (*ptr)(char **, const char **, jobs_t **, int *);
} jobs_builtins_t;

jobs_t *init_jobs_struct(void);
void free_jobs_struct(jobs_t *jobs);
size_t jobs_struct_len(jobs_t *jobs);
void add_elements(jobs_t **jobs, const char *name, pid_t pid, states_t state);
void jobs_command(char **args, const char **env, jobs_t **jobs,
    int *last_return);
void fg_command(char **args, const char **env, jobs_t **jobs, int *last_return);
void bg_command(char **args, const char **env, jobs_t **jobs, int *last_return);
void job_control_synonym(char **args, int *last_return, const char **env,
    jobs_t **jobs);
jobs_t *get_jobs(const char *arg, jobs_t *jobs, char **args, size_t *pos);

static const jobs_builtins_t jobs_builtins[] = {
    {JOBS_FG_CMD, fg_command},
    {JOBS_BG_CMD, bg_command},
    {JOBS_CMD, jobs_command},
    {NULL, NULL}
};

#endif /* JOBS_H_ */
