#include "headerfiles/definitions.h"

int update_status(job jobs[MAXN], int N) {
    int completed = 0; // num jobs completed
    int waitstatus = 1; // initialized to silence warnings

    for (int i = 0; i < N; i++) {
        if ((jobs[i].status == RUNNING) || (jobs[i].status == PAUSED)) {
            waitpid(jobs[i].PID, &waitstatus, WNOHANG);

            if (waitstatus == 0) {
                jobs[i].status = FINISHED;
                // if (DEBUG) printf("--- completed -- %d\n", jobs[i].PID);
                completed += 1;
            }
            waitstatus = 1;
        }
    }
    return completed;
}
