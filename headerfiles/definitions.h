#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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

#define IO 0 // 1: display prompts, 0: no display
#define DEBUG 0 // 1: print extra info when running, 0: don't print info
#define MAXN 30 // max array size
#define NAMESIZE 32 // max length of process names
#define FAKE_DMESG 0 // 1: print desired dmesg output (not part of syscall), 0: run program as normal

enum policy_type {FIFO, RR, SJF, PSJF};
enum jobstatus {UNAVAILABLE, READY, RUNNING, PAUSED, FINISHED};

struct jobstruct {
    int PID;
    char name[NAMESIZE];
    int ready_time;
    int exec_time;
    enum jobstatus status;
};

typedef struct jobstruct job;


#endif // DEFINITIONS_H