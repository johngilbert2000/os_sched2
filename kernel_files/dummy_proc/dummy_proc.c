
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/ktime.h>

SYSCALL_DEFINE1(dummy_proc, int times, int *global_steps) //, int times, int *global_steps)
{
	struct timespec start_time, end_time;
	volatile int t;
	volatile long i;

	getnstimeofday(&start_time);

	for (t = 0; t < times; t++) {
		for (i = 0; i < 1000000UL; i++);
		*global_steps += 1;
	}

	getnstimeofday(&end_time);

	printk(
		"[Project1] %ld %ld.%ld %ld.%ld \n",
		(long)task_pid_nr(current),
		(long)start_time.tv_sec,
		start_time.tv_nsec,
		(long)end_time.tv_sec,
		end_time.tv_nsec);

	return 0;
}
