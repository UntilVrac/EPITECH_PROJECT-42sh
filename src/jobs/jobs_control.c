/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** jobs_control
*/

#include "functions.h"
#include "jobs.h"

static const char *get_state(states_t state)
{
    switch (state) {
        case DONE:
            return "Done";
        case STOPPED:
            return "Suspended";
        case BACKGROUND:
            return "Suspended (tty input)";
        default:
            return "NULL";
    }
}

void jobs_command(char **args, const char **env, jobs_t **jobs_ptr,
    int *last_return)
{
    char symbol = ' ';
    jobs_t *jobs = *jobs_ptr;

    (void)args;
    (void)env;
    *last_return = 0;
    for (size_t i = 0; jobs[i].state != NULL_STATE
        && jobs[i].state != EXITED; i++) {
        symbol = ' ';
        if (jobs[i + 1].state == NULL_STATE)
            symbol = '+';
        if (symbol != '+' && jobs[i + 2].state == NULL_STATE)
            symbol = '-';
        printf("[%zu] %c %s\t\t%s\n", jobs[i].pos, symbol,
            get_state(jobs[i].state), jobs[i].name);
    }
}

void add_elements(jobs_t **jobs, const char *name, pid_t pid, states_t state)
{
    jobs_t *tmp = *jobs;
    size_t len = jobs_struct_len(tmp);

    if (!tmp)
        return;
    tmp = realloc(tmp, sizeof(jobs_t) * (len + 2));
    if (!tmp)
        exit(84);
    tmp[len + 1] = tmp[len];
    tmp[len].pid = pid;
    if (name)
        tmp[len].name = strdup(name);
    else
        tmp[len].name = NULL;
    tmp[len].state = state;
    if (len > 0)
        tmp[len].pos = tmp[len - 1].pos + 1;
    else
        tmp[len].pos = 1;
    *jobs = tmp;
}

size_t jobs_struct_len(jobs_t *jobs)
{
    size_t i = 0;

    if (!jobs)
        return 0;
    while (jobs[i].state != NULL_STATE && jobs[i].state != EXITED)
        i++;
    return i;
}

void free_jobs_struct(jobs_t *jobs)
{
    if (!jobs)
        return;
    for (size_t i = 0; jobs[i].state != NULL_STATE && jobs[i].state != EXITED;
        i++)
        free(jobs[i].name);
    free(jobs);
}

jobs_t *init_jobs_struct(void)
{
    jobs_t *jobs = malloc(sizeof(jobs_t));
    jobs_t tmp_jobs = {0};

    if (!jobs)
        return NULL;
    jobs[0] = tmp_jobs;
    jobs[0].state = NULL_STATE;
    return jobs;
}
