#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>
#include <stdbool.h>

#define MAX_JOBS 64

typedef struct {
    int job_id;
    pid_t pid;
    char command[1024];
    bool active;
} BackgroundJob;

void init_jobs();
void add_background_job(int job_id, pid_t pid, const char *cmd_str);
void cleanup_background_jobs();

#endif