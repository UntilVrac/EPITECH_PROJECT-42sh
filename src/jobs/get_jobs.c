/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** get_jobs
*/

#include "functions.h"
#include "jobs.h"

static jobs_t *check_matching_pattern(const char *arg, jobs_t *jobs,
    char **args, size_t *pos)
{
    size_t nbr = 0;
    size_t start = 0;

    for (size_t i = 0; jobs[i].state != NULL_STATE && jobs[i].state != EXITED;
        i++) {
        if (strstr(jobs[i].name, &arg[2])) {
            nbr++;
            start = i;
            *pos = start;
        }
    }
    if (nbr != 1) {
        if (!nbr)
            fprintf(stderr, "%s: No job matches pattern.\n", args[0]);
        else
            fprintf(stderr, "%s: Ambiguous.\n", args[0]);
        return NULL;
    }
    return &jobs[start];
}

static jobs_t *get_element(size_t val, jobs_t *jobs, size_t *pos)
{
    for (size_t i = 0; jobs[i].state != NULL_STATE && jobs[i].state != EXITED;
        i++) {
        if (jobs[i].pos == val) {
            *pos = i;
            return &jobs[i];
        }
    }
    return NULL;
}

static jobs_t *check_name(const char *arg, jobs_t *jobs, char **args,
    size_t *pos)
{
    size_t nbr = 0;

    for (size_t i = 0; jobs[i].state != NULL_STATE && jobs[i].state != EXITED;
        i++) {
        if (!strncmp(jobs[i].name, &arg[1], strlen(&arg[1]))) {
            nbr++;
            *pos = i;
        }
        if (nbr > 1)
            break;
    }
    if (nbr != 1) {
        if (!nbr)
            fprintf(stderr, "%s: No such job.\n", args[0]);
        else
            fprintf(stderr, "%s: Ambiguous.\n", args[0]);
        return NULL;
    }
    return &jobs[*pos];
}

static jobs_t *check_abbreviations(const char *arg, jobs_t *jobs, char **args,
    size_t *pos)
{
    size_t last = jobs_struct_len(jobs);

    if (!strcmp(arg, "%") || !strcmp(arg, "%+") || !strcmp(arg, "%%")) {
        *pos = last - 1;
        return &jobs[*pos];
    }
    if (!strcmp(arg, "%-")) {
        if (last < 2) {
            fprintf(stderr, "%s: No previous job.\n", args[0]);
            *pos = 1;
            return NULL;
        }
        *pos = last - 2;
        return &jobs[*pos];
    }
    return NULL;
}

static jobs_t *check_pattern(const char *arg, jobs_t *jobs, char **args,
    size_t *pos)
{
    size_t last = jobs_struct_len(jobs);
    int val = 0;
    jobs_t *tmp = check_abbreviations(arg, jobs, args, pos);

    if (!tmp && *pos != 0)
        return NULL;
    else if (tmp)
        return tmp;
    if (str_isnum(&arg[1], &val)) {
        if (val >= (int) jobs[last - 1].pos || val < 1
            || !get_element(val, jobs, pos)) {
            fprintf(stderr, "%s: No such job.\n", args[0]);
            return NULL;
        }
        return &jobs[*pos];
    }
    return check_name(arg, jobs, args, pos);
}

jobs_t *get_jobs(const char *arg, jobs_t *jobs, char **args, size_t *pos)
{
    size_t len = jobs_struct_len(jobs);

    if (arg[0] != '%') {
        *pos = len - 1;
        return &jobs[len - 1];
    } else {
        if (arg[1] == '?')
            return check_matching_pattern(arg, jobs, args, pos);
        else
            return check_pattern(arg, jobs, args, pos);
    }
}
