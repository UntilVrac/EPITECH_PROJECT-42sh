/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** remove and add element to jobs_t struct
*/

#include "jobs.h"
#include <stdlib.h>
#include <string.h>

void remove_element(jobs_t **jobs, size_t pos)
{
    jobs_t *tmp = NULL;
    size_t len = 0;

    if (!jobs || !*jobs)
        return;
    tmp = *jobs;
    len = jobs_struct_len(tmp);
    if (pos >= len)
        return;
    free(tmp[pos].name);
    for (size_t i = pos; i < len; i++)
        tmp[i] = tmp[i + 1];
    tmp = realloc(tmp, sizeof(jobs_t) * len);
    if (!tmp)
        exit(84);
    *jobs = tmp;
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
