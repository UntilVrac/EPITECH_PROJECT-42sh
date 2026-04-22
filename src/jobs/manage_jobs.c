/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** jobs_control
*/

#include "lang.h"
#include "functions.h"
#include "jobs.h"

void bg_command(char **args, const char **env, jobs_t **jobs, int *last_return)
{
    size_t start = 0;

    if (!strcmp(args[0], "bg"))
        start++;
    (void)start;
    (void)env;
    (void)jobs;
    (void)last_return;
}

static void continue_process_parent(jobs_t *jobs, int *last_return,
    const char **env)
{
    int status = 0;

    tcsetpgrp(STDIN_FILENO, jobs->pid);
    waitpid(jobs->pid, &status, WUNTRACED);
    tcsetpgrp(STDIN_FILENO, getpgrp());
    jobs->state = DONE;
    if (WIFSTOPPED(status)) {
        printf("\nSuspended\n");
        jobs->state = STOPPED;
    }
    if (WIFEXITED(status)) {
        *last_return = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        handle_signal_error(status, last_return, env);
    }
}

static void continue_process(jobs_t *jobs, int *last_return, const char **env,
    char **args)
{
    pid_t fork_pid = fork();

    if (fork_pid == 0) {
        setpgid(jobs->pid, jobs->pid);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        printf("%s\n", jobs->name);
        kill(jobs->pid, SIGCONT);
        free_array((char **)(env));
        free_array(args);
        exit(0);
    } else {
        continue_process_parent(jobs, last_return, env);
    }
}

static void remove_element(jobs_t **jobs, size_t pos)
{
    jobs_t *tmp = *jobs;
    size_t len = jobs_struct_len(tmp);

    if (pos >= len)
        return;
    free(tmp[pos].name);
    for (size_t i = pos; i < len; i++)
        tmp[i] = tmp[i + 1];
    tmp = realloc(tmp, sizeof(jobs_t) * len);
    *jobs = tmp;
}

static bool is_empty_struct(jobs_t *jobs, char **args, int *last_return)
{
    if (!jobs || jobs[0].state == EXITED || jobs[0].state == NULL_STATE) {
        fprintf(stderr, "%s: No current job.\n", args[0]);
        *last_return = 1;
        return true;
    }
    return false;
}

static jobs_t *check_args_jobs(char **args, jobs_t **jobs, size_t *i,
    const char *command)
{
    size_t start = 0;
    jobs_t *tmp = *jobs;

    if (!args)
        return NULL;
    if (!strcmp(args[0], command))
        start++;
    if (!args[start]) {
        *i = jobs_struct_len(*jobs) - 1;
        return &(tmp[*i]);
    } else
        return get_jobs(args[start], *jobs, args, i);
}

void fg_command(char **args, const char **env, jobs_t **jobs, int *last_return)
{
    jobs_t *tmp = NULL;
    size_t i = 0;

    *last_return = 0;
    if (is_empty_struct(*jobs, args, last_return))
        return;
    tmp = check_args_jobs(args, jobs, &i, "fg");
    if (!tmp) {
        *last_return = 1;
        return;
    }
    continue_process(tmp, last_return, env, args);
    if (tmp->state == DONE) {
        remove_element(jobs, i);
        if (!*jobs)
            exit(84);
    }
}

void job_control_synonym(char **args, int *last_return, const char **env,
    jobs_t **jobs)
{
    *last_return = 0;
    if (args[1] == NULL)
        return fg_command(args, env, jobs, last_return);
    else if (!strcmp(args[1], "&"))
        return bg_command(args, env, jobs, last_return);
    else {
        print_error(args[0], TOO_M_ARGS, env);
        *last_return = 1;
    }
}
