#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h> // for kill()
#include <sys/mman.h> // for mmap()
#include <wait.h> // for waitpid

#include <sched.h> // to set to one core (sched_setaffinity) 
#include <sys/wait.h>

#include "headerfiles/definitions.h"
#include "headerfiles/update_status.h"

extern int *global_steps;


long double get_time(){
    // gets the clock time in nanoseconds (from time.h)
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (t.tv_sec*10E9 + t.tv_nsec);
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
            long double start_time = get_time() * 10E-9;

            // Set to one core
            cpu_set_t single_cpu; // cpu bit mask
            CPU_ZERO(&single_cpu); // initialize to 0
            CPU_SET(1, &single_cpu); // set to desired number of cores (1)
            sched_setaffinity(PID, sizeof(cpu_set_t), &single_cpu); // set process to single core

            int debug_sys;
            if (SYSDEBUG) {
                debug_sys = 1;
            } else {
                debug_sys = 0;
            }
            
            #ifdef LINUX
            syscall(436, jobs[id].exec_time, &global_steps, debug_sys);
            #else
            for (int i = 0; i < jobs[id].exec_time; i++) {
                time_unit();
                *global_steps += 1;
            }
            #endif

            long double stop_time = get_time() * 10E-9;

            if (DEBUG || FAKE_DMESG) printf("process %d -- start: %Lf -- stop: %Lf -- elapsed: %Lf -- steps: %d\n", \
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
        jobs[id].status = RUNNING;
        kill(jobs[id].PID, SIGCONT);
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
    kill(jobs[id].PID, SIGSTOP);
}
