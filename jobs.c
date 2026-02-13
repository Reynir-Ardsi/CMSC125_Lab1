#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "jobs.h"

BackgroundJob bg_jobs[MAX_JOBS];

void init_jobs() {
    for (int i = 0; i < MAX_JOBS; i++) {
        bg_jobs[i].active = false;
    }
}

void add_background_job(int job_id, pid_t pid, const char *cmd_str) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!bg_jobs[i].active) {
            bg_jobs[i].job_id = job_id;
            bg_jobs[i].pid = pid;
            strncpy(bg_jobs[i].command, cmd_str, sizeof(bg_jobs[i].command) - 1);
            bg_jobs[i].command[sizeof(bg_jobs[i].command) - 1] = '\0';
            bg_jobs[i].active = true;
            return;
        }
    }
    fprintf(stderr, "mysh: warning: maximum background jobs reached\n");
}

void cleanup_background_jobs() {
    int status;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (bg_jobs[i].active) {
            pid_t result = waitpid(bg_jobs[i].pid, &status, WNOHANG);
            
            if (result > 0) {
                printf("[%d] Done\t%s\n", bg_jobs[i].job_id, bg_jobs[i].command);
                bg_jobs[i].active = false;
            } 
            else if (result < 0) {
                bg_jobs[i].active = false;
            }
        }
    }
}