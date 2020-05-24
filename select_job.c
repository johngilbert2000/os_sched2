#include "headerfiles/definitions.h"

bool status_check(enum jobstatus stat) {
    if ((stat == UNAVAILABLE) || (stat == FINISHED)) {
        return false;
    } 
    else if ((stat == READY) || (stat == RUNNING) || (stat == PAUSED)) {
        return true;
    }
    else {
        return false;
    }
}

int select_job(enum policy_type policy, job jobs[MAXN], bool running, int current_id, int current_step) {
    int id = -1; // id<0 indicates no job selected

    if (policy == FIFO) {
        // not preemptive
        if (running) return current_id;

        int min_rtime = 999999999; // minimum ready time
        bool check;
        for (int i = 0; i < MAXN; i++) {
            check = status_check(jobs[i].status);
            if (check) {
                if (jobs[i].ready_time < min_rtime) {
                    min_rtime = jobs[i].ready_time;
                    id = i;
                }
            }
        }
        return id;
    }
    else if (policy == RR) {
        // preemptive
        int elapsed_time = 0;
        int next_id = (current_id + 1) % MAXN;
        bool check;

        if (running) {
            // Rotate jobs every 500 time steps
            elapsed_time = current_step - jobs[current_id].ready_time;

            if (elapsed_time % 500 != 0) {
                // Don't rotate
                if (DEBUG) printf("(don't rotate)\n");
                return current_id;
            }

            // Rotate
            while (next_id != current_id) {
                check = status_check(jobs[next_id].status);
                if (check) {
                    if (DEBUG) printf("(rotate)\n");
                    return next_id;
                }
                next_id = (next_id + 1) % MAXN;
            }
            return current_id;
        }

        // If no jobs are running, just do FIFO
        int min_rtime = 999999999; // minimum ready time
        for (int i = 0; i < MAXN; i++) {
            check = status_check(jobs[i].status);
            if (check) {
                if (jobs[i].ready_time < min_rtime) {
                    min_rtime = jobs[i].ready_time;
                    id = i;
                }
            }
        }
        // if (DEBUG) printf("(fifo) -- %d\n", id);
        return id;
    }
    else if (policy == SJF) {
        // nonpreemptive
        if (running) return current_id;

        int min_extime = 999999999; // minimum execution time
        int min_rtime = 999999999; // minimum ready time (in case of a tie)
        bool check;
        for (int i = 0; i < MAXN; i++) {
            check = status_check(jobs[i].status);
            if (check) {
                if (jobs[i].exec_time < min_extime) {
                    min_extime = jobs[i].exec_time;
                    min_rtime = jobs[i].ready_time;
                    id = i;
                }
                else if (jobs[i].exec_time == min_extime) {
                    // choose the job that arrived first in event of a tie
                    if (jobs[i].ready_time < min_rtime) {
                        min_rtime = jobs[i].ready_time;
                        id = i;
                    }
                }
            }
        }
        return id;
    }
    else if (policy == PSJF) {
        // preemptive
        int min_extime = 999999999; // minimum execution time
        int min_rtime = 999999999; // minimum ready time (in case of a tie)
        bool check;
        for (int i = 0; i < MAXN; i++) {
            check = status_check(jobs[i].status);
            if (check) {
                if (jobs[i].exec_time < min_extime) {
                    min_extime = jobs[i].exec_time;
                    min_rtime = jobs[i].ready_time;
                    id = i;
                }
                else if (jobs[i].exec_time == min_extime) {
                    // choose the job that arrived first in event of a tie
                    if (jobs[i].ready_time < min_rtime) {
                        min_rtime = jobs[i].ready_time;
                        id = i;
                    }
                }
            }
        }
        return id;
    }
    else {
        if (DEBUG) printf("No valid policy for job selection\n");
        exit(EXIT_FAILURE);
    }
}