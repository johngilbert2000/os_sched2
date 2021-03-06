#include "headerfiles/definitions.h"
#include "headerfiles/get_policy.h"
#include "headerfiles/select_job.h"
#include "headerfiles/run_job.h"
#include "headerfiles/update_status.h"


int *global_steps;

void half_time_unit(){
    // used so main.c will check processes faster than they run
    // edit: unable to share state with new syscall, so this is not used
    volatile unsigned long i; for(i=0;i<500000UL;i++); 
}


int main() {
    enum policy_type policy;

    global_steps = (int*)mmap(NULL, sizeof *global_steps, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *global_steps = 0;

    
    // Set to one core
    cpu_set_t single_cpu; // cpu bit mask
    CPU_ZERO(&single_cpu); // initialize to 0
    CPU_SET(1, &single_cpu); // set to desired number of cores (1)
    sched_setaffinity(getpid(), sizeof(cpu_set_t), &single_cpu); // set process to single core

    // -----------------------
    // Get policy
    // -----------------------
    policy = get_policy();

    int N;
    if (IO) printf("Number of processes ");
    scanf("%u", &N);


    // -----------------------
    // Obtain Input
    // -----------------------
    job jobs[MAXN];

    // N R T
    for (int i = 0; i < N; i++) { 
        if (IO) printf("N R T: ");
        scanf("%s", jobs[i].name); 
        scanf("%u", &jobs[i].ready_time);
        scanf("%u", &jobs[i].exec_time);

    }

    for (int i = 0; i < MAXN; i++) {
        jobs[i].PID = -1;
        jobs[i].status = UNAVAILABLE;

        if (i >= N) {
            jobs[i].exec_time = 0;
            jobs[i].ready_time = 999999;
            strncpy(jobs[i].name, "NA", NAMESIZE);
        }
    }

    // -----------------------
    // Initialize Parameters
    // -----------------------
    int jobs_complete = 0;
    int step = 0; // current time step
    bool running; // indicates if previous process is running
    int id; // id of current job
    int id_prev; // id of previous job

    running = false;
    id = -1;
    id_prev = -1;

    if (DEBUG) {
        for (int i = 0; i < N; i++) {
            printf("%s %d %d\n", jobs[i].name, jobs[i].ready_time, jobs[i].exec_time);
        }
    }

    // -----------------------
    // Initiate loop
    // -----------------------
    while (jobs_complete < N) {
        if (DEBUG) printf("----------\nSTEP: %d (main: %d)\n", *global_steps, step);



        // -----------------------
        // Add Ready Jobs
        // -----------------------
        for (int i = 0; i < N; i++){
            if ((jobs[i].status == UNAVAILABLE) && (jobs[i].ready_time <= *global_steps)) {
                jobs[i].status = READY;
                if (DEBUG) printf("adding job -- %s\n", jobs[i].name);
            }
        }

        // -----------------------
        // Select Job
        // -----------------------
        id = select_job(policy, jobs, running, id, *global_steps);
        if (DEBUG && (id > 0)) printf("select job -- %s\n", jobs[id].name);
        
        if (id < 0) {
            // No job selected
            if (DEBUG) printf("no job\n");
            time_unit();
            step += 1;
            *global_steps += 1;
            jobs_complete += update_status(jobs, N);
            running = false;
            continue;
        }
        else {
            // -----------------------
            // Run Job
            // -----------------------
            if ((running) && (id != id_prev)) {
                if (DEBUG) printf("pausing %s\n", jobs[id_prev].name);
                pause_job(jobs, id_prev);
            }
            jobs_complete += run_job(jobs, id);
            running = true;
            id_prev = id;

            if (DEBUG) printf("run job -- %s (%d) \n", jobs[id].name, jobs[id].PID);

            // -----------------------
            // Update Status
            // -----------------------
            half_time_unit();
            step += 1;

            #ifdef LINUX
            // syscall doesn't seem to take the global_steps variable
 //           half_time_unit();
            if (*global_steps < (step)) {
//                *global_steps = step;
            }
            #endif

            jobs_complete += update_status(jobs, N);
            if (jobs[id].status == FINISHED) running = false;

            // -----------------------
            // Debug Info
            // -----------------------
            if (DEBUG) {
                printf("running %d\n", running);

                printf("ready: ");
                for (int i = 0; i < MAXN; i++) {
                    if (jobs[i].status == READY) printf("%s ", jobs[i].name);
                }
                printf("\n");

               printf("paused: ");
                for (int i = 0; i < MAXN; i++) {
                    if (jobs[i].status == PAUSED) printf("%s ", jobs[i].name);
                }
                printf("\n");

                printf("running: ");
                for (int i = 0; i < MAXN; i++) {
                    if (jobs[i].status == RUNNING) printf("%s ", jobs[i].name);
                }
                printf("\n");


                printf("completed: ");
                for (int i = 0; i < MAXN; i++) {
                    if (jobs[i].status == FINISHED) printf("%s ", jobs[i].name);
                }
                printf("\n");

                printf("STEP: %d\n", *global_steps);
            }

        }
        if (DEBUG && (step > 200000)) break; // use shorter loops when debugging
    }

    // ------------------
    // Output
    // ------------------
    for (int i = 0; i < N; i++) {
        printf("%s %d\n", jobs[i].name, jobs[i].PID);
    }
}
