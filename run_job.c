#include "headerfiles/definitions.h"
#include "headerfiles/update_status.h"

extern int *global_steps;


long double get_time(){
    // gets the clock time in nanoseconds (from time.h)
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (t.tv_sec + t.tv_nsec);
}

void time_unit(){
    volatile unsigned long i; for(i=0;i<1000000UL;i++); 
}

int run_job(job jobs[MAXN], int id) {
    if (jobs[id].status == READY) {
    // Start Job
        int PID = fork();
        if (PID == 0) {
            int PID = getpid();
            long double start_time = get_time() * 10E-7;

            
            
            #ifdef LINUX
            syscall(436, J.exec_time);
            #else
            for (int i = 0; i < jobs[id].exec_time; i++) {
                time_unit();
                *global_steps += 1;
            }
            #endif

            long double stop_time = get_time() * 10E-7;

            if (DEBUG) printf("process %d -- start: %Lf -- stop: %Lf -- elapsed: %Lf -- steps: %d\n", \
            PID, start_time, stop_time, stop_time - start_time, jobs[id].exec_time);

            exit(EXIT_SUCCESS);
        }
        else {
            jobs[id].status = RUNNING;
            jobs[id].PID = PID;

            if (DEBUG) printf("starting process %d\n", jobs[id].PID);
        }
    }
    else if (jobs[id].status == PAUSED) {
    // Continue Job
        if (DEBUG) printf("continue process %d\n", jobs[id].PID);
        if (USE_KILL) kill(jobs[id].PID, SIGCONT);
        jobs[id].status = RUNNING;
    }

    int waitstatus = 1;
    waitpid(jobs[id].PID, &waitstatus, WNOHANG);
    if (waitstatus == 0) {
        if (DEBUG) printf("PROCESS COMPLETE %d\n", jobs[id].PID);
        jobs[id].status = FINISHED;
        return 1;
    }
    else {
        return 0;
    }

}

void pause_job(job jobs[MAXN], int id) {
    if (DEBUG) printf("pausing process %d\n", jobs[id].PID);
    jobs[id].status = PAUSED;
    if (USE_KILL) kill(jobs[id].PID, SIGSTOP);
}