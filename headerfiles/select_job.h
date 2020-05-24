#ifndef SELECT_JOB_H
#define SELECT_JOB_H

#include "definitions.h"

extern int select_job(enum policy_type policy, job jobs[MAXN], bool running, int current_id, int current_time);

#endif