CC=gcc
CFLAGS=-g -fsanitize=address 
# -Wall -Wuninitialized

all: sched2

%.o: %.c
	$(CC) $(CFLAGS) -c $^

sched2: get_policy.o run_job.o select_job.o update_status.o main.c
	$(CC) $(CFLAGS) -o $@ $^
	rm *.o
	
run:
	@./sched2

scratchwork:
	$(CC) scratchwork.c
	@./a.out
	@rm a.out

clean:
	@rm sched2
	@rm -r *.dSYM/
	@rm *.o

cleanall:
	@rm sched2
	@rm -r *.dSYM/
	@rm -r output/*.txt
	@rm *.o