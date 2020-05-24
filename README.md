# os_sched2

A continuation of my userspace OS Scheduler project, (original version found [here](https://github.com/johngilbert2000/OS_scheduler))

I managed to fix all the issues with my original project by switching from linked lists to arrays.

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


### Demo

A demo can be found at this link: ______


The following commands were used to produce the files in the `output` folder:

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
