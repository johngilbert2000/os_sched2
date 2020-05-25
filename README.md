# OS Scheduler

<hr>

A continuation of my userspace OS Scheduler project, (original version found [here](https://github.com/johngilbert2000/OS_scheduler))

I managed to fix all the issues with my original project by switching from linked lists to arrays.

This scheduler has the following policies, designed to run on Linux:

- FIFO (First In First Out)
- RR (Round Robin)
- SJF (Shortest Job First)
- PSJF (Preemptive Shortest Job First)

More details can be found in *report.pdf*

<hr>

### Demo

A demo can be found at this link: https://www.youtube.com/watch?v=ZaO9pVGO7to

*Note*: To ensure the kernel compiled correctly, I recommend temporarily switching `DEBUG` and `SYSDEBUG` to `1` in *headerfiles/definitions.h* to check that the time steps are updating correctly from the processes.

Debug example: https://www.youtube.com/watch?v=Gj3yPg6UDzY

<hr>

### Compiling the Kernel

On a Linux machine, do the following to compile the custom kernel, (uses kernel v5.4.35). Note that this takes some time.
```
sudo apt-get update

sudo apt-get install git build-essential fakeroot libncurses5-dev libssl-dev ccache libncurses-dev bison flex gcc make git vim

git clone https://github.com/johngilbert2000/os_sched2.git

wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.4.35.tar.xz
tar xf linux-5.4.35.tar.xz

cd linux-5.4.35/

make mrproper
cp /boot/config-$(uname -r) .config
yes '' | make oldconfig

cp ~/os_sched2/kernel_files/syscall_32.tbl arch/x86/entry/syscalls/syscall_32.tbl
cp ~/os_sched2/kernel_files/syscall_64.tbl arch/x86/entry/syscalls/syscall_64.tbl
cp ~/os_sched2/kernel_files/unistd.h include/uapi/asm-generic/unistd.h
cp ~/os_sched2/kernel_files/syscalls.h include/linux/syscalls.h
cp ~/os_sched2/kernel_files/sys_ni.c kernel/sys_ni.c
cp -r ~/os_sched2/kernel_files/dummy_proc dummy_proc
cp ~/os_sched2/kernel_files/Makefile Makefile

make -j12
make modules_install
make install
update-grub
sudo reboot
```
<hr>

### Compiling and Running the Scheduler

Compile:
```
make
```

Run:
```
make run
```

Run with given inputs, and store outputs to a file:
```
make run < input/FIFO_1.txt > output/FIFO_1_stdout.txt
```

Add dmesg info to file:
```
dmesg | grep "Project1" > output/FIFO_1_dmesg.txt
sudo dmesg --clear
```

When finished:
```
make clean
```
<hr>

### Input Format

The program gets input parameters from the standard input in the following format

```
S // the scheduling policy, one of the following strings: FIFO, RR, SJF, PSJF.
N // the number of processes
N1 R1 T1
N2 R2 T2
…
Nn Rn Tn
//Ni - a character string with a length less than 32 bytes, specifying the name of the i-th process.
//Ri - a non-negative integer, specifying the ready time of the i-th process.
//Ti - a non-negative integer, specifying the execution time of the i-th process.
```

For example, input may look like:
```
FIFO
7
P1 0 8000
P2 200 5000
P3 300 3000
P4 400 1000
P5 500 1000
P6 500 1000
P7 600 4000
```

<hr>

### Output Format

Standard output of the program will have the following format:

```
NAME PID
//NAME - the name of this process specified by the input
//PID - the process id of this process
In the Linux kernel, start and stop times will be displayed as follows:

TAG PID ST FT
//TAG - the tag to identify the message of this project.
//PID - the process ID of this process
//ST - the start time of this process in the format seconds.nanoseconds.
//FT - the finish time of this process in the format seconds.nanoseconds.
```

An example output:

```
P1 2778
P2 2792
P3 2795
P4 2797
P5 2799
P6 2801
P7 2803
```

And output from the command dmesg (`dmesg | grep "Project1"`) might contain:
```
[ 1327.722260] [Project1] 2778 1590424612.90920294 1590424627.946372441 
[ 1353.515798] [Project1] 2792 1590424643.834925785 1590424653.741548037 
[ 1369.403974] [Project1] 2795 1590424663.682937327 1590424669.630731657 
[ 1377.364074] [Project1] 2797 1590424675.610933604 1590424677.591339789 
[ 1381.361645] [Project1] 2799 1590424679.606968779 1590424681.589164933 
[ 1385.363193] [Project1] 2801 1590424683.606940364 1590424685.590967164 
[ 1395.314163] [Project1] 2803 1590424687.610925387 1590424695.542570747 
```

<hr>

### Output Commands

Here's a a screenshot of commands used to produce the files in the `output` folder:

![IO_screenshot](https://github.com/johngilbert2000/os_sched2/blob/master/demo/outputs_img.png)


Here's the full list of commands used to produce the files in the `output` folder:

```
sudo dmesg --clear

make
make run < input/FIFO_1.txt > output/FIFO_1_stdout.txt
dmesg | grep "Project1" > output/FIFO_1_dmesg.txt

sudo dmesg --clear

make run < input/FIFO_2.txt > output/FIFO_2_stdout.txt
dmesg | grep "Project1" > output/FIFO_2_dmesg.txt

sudo dmesg --clear

make run < input/FIFO_3.txt > output/FIFO_3_stdout.txt
dmesg | grep "Project1" > output/FIFO_3_dmesg.txt

sudo dmesg --clear

make run < input/FIFO_4.txt > output/FIFO_4_stdout.txt
dmesg | grep "Project1" > output/FIFO_4_dmesg.txt

sudo dmesg --clear

make run < input/FIFO_5.txt > output/FIFO_5_stdout.txt
dmesg | grep "Project1" > output/FIFO_5_dmesg.txt

sudo dmesg --clear

make run < input/PSJF_1.txt > output/PSJF_1_stdout.txt
dmesg | grep "Project1" > output/PSJF_1_dmesg.txt

sudo dmesg --clear

make run < input/PSJF_2.txt > output/PSJF_2_stdout.txt
dmesg | grep "Project1" > output/PSJF_2_dmesg.txt

sudo dmesg --clear

make run < input/PSJF_3.txt > output/PSJF_3_stdout.txt
dmesg | grep "Project1" > output/PSJF_3_dmesg.txt

sudo dmesg --clear

make run < input/PSJF_4.txt > output/PSJF_4_stdout.txt
dmesg | grep "Project1" > output/PSJF_4_dmesg.txt

sudo dmesg --clear

make run < input/PSJF_5.txt > output/PSJF_5_stdout.txt
dmesg | grep "Project1" > output/PSJF_5_dmesg.txt

sudo dmesg --clear

make run < input/SJF_1.txt > output/SJF_1_stdout.txt
dmesg | grep "Project1" > output/SJF_1_dmesg.txt

sudo dmesg --clear

make run < input/SJF_2.txt > output/SJF_2_stdout.txt
dmesg | grep "Project1" > output/SJF_2_dmesg.txt

sudo dmesg --clear

make run < input/SJF_3.txt > output/SJF_3_stdout.txt
dmesg | grep "Project1" > output/SJF_3_dmesg.txt

sudo dmesg --clear

make run < input/SJF_4.txt > output/SJF_4_stdout.txt
dmesg | grep "Project1" > output/SJF_4_dmesg.txt

sudo dmesg --clear

make run < input/SJF_5.txt > output/SJF_5_stdout.txt
dmesg | grep "Project1" > output/SJF_5_dmesg.txt

sudo dmesg --clear

make run < input/RR_1.txt > output/RR_1_stdout.txt
dmesg | grep "Project1" > output/RR_1_dmesg.txt

sudo dmesg --clear

make run < input/RR_2.txt > output/RR_2_stdout.txt
dmesg | grep "Project1" > output/RR_2_dmesg.txt

sudo dmesg --clear

make run < input/RR_3.txt > output/RR_3_stdout.txt
dmesg | grep "Project1" > output/RR_3_dmesg.txt

sudo dmesg --clear

make run < input/RR_4.txt > output/RR_4_stdout.txt
dmesg | grep "Project1" > output/RR_4_dmesg.txt

sudo dmesg --clear

make run < input/RR_5.txt > output/RR_5_stdout.txt
dmesg | grep "Project1" > output/RR_5_dmesg.txt

sudo dmesg --clear

make run < input/TIME_MEASUREMENT.txt > output/TIME_MEASUREMENT_stdout.txt
dmesg | grep "Project1" > output/TIME_MEASUREMENT_dmesg.txt
```
