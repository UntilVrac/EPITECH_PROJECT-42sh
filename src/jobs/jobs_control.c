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
        case BG_STOP_IN:
            return "Suspended (tty input)";
        case BG_STOP_OUT:
            return "Suspended (tty output)";
        case RUNNING:
            return "Running";
        default:
            return "NULL";
    }
}

static void print_jobs(jobs_t *jobs, size_t pos)
{
    char symbol = ' ';

    if (!jobs || jobs[pos].state == NULL_STATE || jobs[pos].state == EXITED)
        return;
    if (jobs[pos + 1].state == NULL_STATE)
        symbol = '+';
    if (symbol != '+' && jobs[pos + 2].state == NULL_STATE)
        symbol = '-';
    printf("[%zu] %c %s\t\t%s\n", jobs[pos].pos, symbol,
        get_state(jobs[pos].state), jobs[pos].name);
}

static void check_background_status(jobs_t *tmp, size_t pos, int status)
{
    if (WIFSTOPPED(status)) {
        tmp[pos].state = STOPPED;
        if (WSTOPSIG(status) == SIGTTIN)
            tmp[pos].state = BG_STOP_IN;
        if (WSTOPSIG(status) == SIGTTOU)
            tmp[pos].state = BG_STOP_OUT;
    } else
        tmp[pos].state = DONE;
    print_jobs(tmp, pos);
    if (tmp[pos].state == DONE)
        remove_element(&tmp, pos);
}

void check_background_jobs(jobs_t **jobs)
{
    jobs_t *tmp = NULL;
    int status = 0;
    pid_t pid = 0;
    size_t i = 0;

    if (jobs == NULL || *jobs == NULL)
        return;
    tmp = *jobs;
    pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED);
    while (pid > 0) {
        i = get_jobs_by_pid(tmp, pid);
        if (tmp[i].state != EXITED && tmp[i].state != NULL_STATE)
            check_background_status(tmp, i, status);
        pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED);
    }
}

void jobs_command(char **args, const char **env, jobs_t **jobs_ptr,
    int *last_return)
{
    jobs_t *jobs = *jobs_ptr;

    (void)args;
    (void)env;
    (void)last_return;
    for (size_t i = 0; jobs[i].state != NULL_STATE
        && jobs[i].state != EXITED; i++) {
        print_jobs(jobs, i);
    }
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
