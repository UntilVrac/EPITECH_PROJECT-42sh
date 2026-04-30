/*
** EPITECH PROJECT, 2026
** G2 - Shell Programming - 42sh
** File description:
** jobs_control
*/

#include "lang.h"
#include "functions.h"
#include "jobs.h"

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
        waitpid(fork_pid, NULL, 0);
    }
}

static void continue_process_bg(jobs_t *jobs, const char **env)
{
    pid_t pid = fork();

    (void)env;
    if (pid == 0) {
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        printf("[%zu]\t%s &\n", jobs->pos, jobs->name);
        kill(jobs->pid, SIGCONT);
        free_array((char **)(env));
        exit(0);
    } else {
        jobs->state = RUNNING;
        waitpid(pid, NULL, 0);
    }
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

static jobs_t *check_args_jobs(char **args, jobs_t **jobs, size_t *pos,
    size_t i)
{
    jobs_t *tmp = *jobs;

    if (!args)
        return NULL;
    if (!args[i]) {
        *pos = jobs_struct_len(*jobs) - 1;
        return &(tmp[*pos]);
    } else
        return get_jobs(args[i], *jobs, args, pos);
}

void bg_command(char **args, const char **env, jobs_t **jobs, int *last_return)
{
    jobs_t *tmp = NULL;
    size_t pos = 0;
    size_t start = !strcmp(args[0], JOBS_BG_CMD) ? 1 : 0;

    do {
        pos = 0;
        if (is_empty_struct(*jobs, args, last_return))
            return;
        tmp = check_args_jobs(args, jobs, &pos, start);
        if (!tmp) {
            *last_return = 1;
            return;
        }
        continue_process_bg(tmp, env);
        if (tmp->state == DONE)
            remove_element(jobs, pos);
        if (!args[start])
            break;
        start++;
    } while (args[start]);
}

void fg_command(char **args, const char **env, jobs_t **jobs, int *last_return)
{
    jobs_t *tmp = NULL;
    size_t pos = 0;
    size_t start = !strcmp(args[0], JOBS_FG_CMD) ? 1 : 0;

    do {
        pos = 0;
        if (is_empty_struct(*jobs, args, last_return))
            return;
        tmp = check_args_jobs(args, jobs, &pos, start);
        if (!tmp) {
            *last_return = 1;
            return;
        }
        continue_process(tmp, last_return, env, args);
        if (tmp->state == DONE)
            remove_element(jobs, pos);
        if (!args[start])
            break;
        start++;
    } while (args[start]);
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
