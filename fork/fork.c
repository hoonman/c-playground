#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void unix_error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg);
    exit(0);
}

pid_t Fork(void) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        unix_error("Fork error");
    }
    return pid;
}

int main() {
    pid_t pid;
    int x = 3;
    pid = Fork();
    // if (pid < 0) {
    //     fprintf(stderr, "fork error: %s\n", strerror(errno));
    // }

    if (pid == 0) {
        printf("child: x=%d\n", ++x);
    }

    // parent
    printf("parent: x=%d\n", --x);
    exit(0);

}


// waitpid:
// return value:
// 


int main(int argc, char* argv[]) {
    // ...existing setup code...
    
    sigset_t mask_child, prev_mask;
    sigemptyset(&mask_child);
    sigaddset(&mask_child, SIGCHLD);

    while ((line = readline(SHELL_PROMPT)) != NULL) {
        job_info* job = validate_input(line);
        if (job == NULL) {
            free(line);
            continue;
        }

        // ...handle built-in commands...

        // BLOCK SIGCHLD before fork
        sigprocmask(SIG_BLOCK, &mask_child, &prev_mask);

        if ((pid = fork()) < 0) {
            perror("fork error");
            sigprocmask(SIG_SETMASK, &prev_mask, NULL);  // restore
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // child
            sigprocmask(SIG_SETMASK, &prev_mask, NULL);  // unblock in child
            proc_info* proc = job->procs;
            exec_result = execvp(proc->cmd, proc->argv);
            if (exec_result < 0) {
                printf(EXEC_ERR, proc->cmd);
                free_job(job);
                free(line);
                validate_input(NULL);
                exit(EXIT_FAILURE);
            }
        } else {  // parent
            if (job->bg) {
                if (bg_jobs->length >= max_bgprocs && max_bgprocs != -1) {
                    fprintf(stderr, BG_ERR);
                    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
                    free_memory(line, job);
                    continue;
                }
                bgentry_t* bg_job = create_bg_job(job, pid);
                InsertInOrder(bg_jobs, bg_job);
                // UNBLOCK after inserting into list
                sigprocmask(SIG_SETMASK, &prev_mask, NULL);
            } else {
                // UNBLOCK before waiting (so SIGCHLD for bg jobs can arrive)
                sigprocmask(SIG_SETMASK, &prev_mask, NULL);
                process_foreground_job(pid, &exit_status, &wait_result);
                free_job(job);
            }
        }

        // Check flag after ANY command completes
        if (child_terminated == 1) {
            clean_background_jobs(bg_jobs, 0);
            child_terminated = 0;
        }

        if (!job->bg) {
            // already freed
        }
        free(line);
    }