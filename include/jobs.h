/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** jobs.h
*/

#ifndef JOBS_H_
    #define JOBS_H_

    #include <sys/types.h>

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

jobs_t *init_jobs_struct(void);
void free_jobs_struct(jobs_t *jobs);
size_t jobs_struct_len(jobs_t *jobs);
void add_elements(jobs_t **jobs, const char *name, pid_t pid, states_t state);
int jobs_command(char **args, jobs_t *jobs);

#endif /* JOBS_H_ */
