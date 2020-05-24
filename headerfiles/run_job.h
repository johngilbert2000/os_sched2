#ifndef RUN_JOB_H
#define RUN_JOB_H

#include "definitions.h"

extern void time_unit();
extern int run_job(job jobs[MAXN], int id);
extern void pause_job(job jobs[MAXN], int id);

#endif