# Demand Paging

## Intro
I had read somewhere (some book or blog, I don't remember) that when a process calls [mmap(2)](https://www.man7.org/linux/man-pages/man2/mmap.2.html) to map an anonymous page, it does not actually allocate memory initially (it has no physical page associated with it).

Rather, only the page table is updated (entries are created), and a corresponding physical page is allocated once the process tries to access an address within the memory region.

The experiments here aim to demonstrate this visually.

The code can be built by running ```make```.

Warning: One of the programs **requires to be run as root**. 


## Experiment 1: Single Page

```singlemap```  does the following:
1. prints its own pid
2. allocates a single page with *mmap(2)*
3. accesses the allocated page

After each step, it waits for the user to press enter before moving on.

We can observe the steps by running ```singlemap```, and then running ```ptdump``` with the pid of the process.

```console
$ ./singlemap
pid: 146251
```
In a different terminal (or split terminal), we run ptdump.
At this point, *mmap(2)* has not been called yet.

```console
$ watch -n 0.1 "sudo ./ptdump/ptdump 146251"

Every 0.1s: sudo ./ptdump/ptdump 146251                                                                                                     cyberia: Sun Dec 17 18:21:31 2023

virt addr              physical addr            size          perms        present        swapped       path    # Some paths have been shortened
0x55d44a4f7000         0x226547000                4096        r--p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4f8000         0x1f6c40000                4096        r-xp         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4f9000         0x11e0ed000                4096        r--p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4fa000         0x275020000                4096        r--p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4fb000         0x1dff1d000                4096        rw-p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a84f000         0x37c94c000              135168        rw-p         true           false         [heap]
0x7f348c547000         0x34d030000               12288        rw-p         true           false
0x7f348c54a000         0x32c80a000              155648        r--p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c570000         0x3bee76000             1396736        r-xp         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c6c5000                 0x0              339968        r--p        false           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c718000         0x364260000               16384        r--p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c71c000         0x287822000                8192        rw-p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c71e000         0x251470000               53248        rw-p         true           false
0x7f348c73f000         0x201642000                8192        rw-p         true           false
0x7f348c741000         0x3bee36000                4096        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c742000         0x274923000              151552        r-xp         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c767000         0x2b320c000               40960        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c771000         0x1a78c5000                8192        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c773000         0x1e5fbd000                8192        rw-p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7ffde154e000                 0x0              135168        rw-p        false           false         [stack]
0x7ffde15f4000                 0x0               16384        r--p        false           false         [vvar]
0x7ffde15f8000         0x330a0b000                8192        r-xp         true           false         [vdso]

```

As we progress the program by pressing enter in the terminal running ```singlemap```, the output of ```ptdump``` should change.

In ```singlemap```, the page is allocated with the permissions rwx so it is easier to find.

After pressing enter:

```console
pid: 146251

Allocated new map: 0x7f348c73e000

```

```console
Every 0.1s: sudo ./ptdump/ptdump 146251                                                                                                     cyberia: Sun Dec 17 18:27:20 2023

virt addr              physical addr            size          perms        present        swapped       path        # Some paths have been shortened
0x55d44a4f7000         0x226547000                4096        r--p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4f8000         0x1f6c40000                4096        r-xp         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4f9000         0x11e0ed000                4096        r--p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4fa000         0x275020000                4096        r--p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a4fb000         0x1dff1d000                4096        rw-p         true           false         .../linux-experiments/demand_paging/singlemap
0x55d44a84f000         0x37c94c000              135168        rw-p         true           false         [heap]
0x7f348c547000         0x34d030000               12288        rw-p         true           false
0x7f348c54a000         0x32c80a000              155648        r--p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c570000         0x3bee76000             1396736        r-xp         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c6c5000                 0x0              339968        r--p        false           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c718000         0x364260000               16384        r--p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c71c000         0x287822000                8192        rw-p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c71e000         0x251470000               53248        rw-p         true           false
0x7f348c73e000                 0x0                4096        rwxp        false           false         <----- New page, with physical addr 0x0
0x7f348c73f000         0x201642000                8192        rw-p         true           false
0x7f348c741000         0x3bee36000                4096        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c742000         0x274923000              151552        r-xp         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c767000         0x2b320c000               40960        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c771000         0x1a78c5000                8192        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c773000         0x1e5fbd000                8192        rw-p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7ffde154e000                 0x0              135168        rw-p        false           false         [stack]
0x7ffde15f4000                 0x0               16384        r--p        false           false         [vvar]
0x7ffde15f8000         0x330a0b000                8192        r-xp         true           false         [vdso]

```

After pressing enter yet another time.

```console
pid: 146251

Allocated new map: 0x7f348c73e000

Writing data to 0x7f348c73e000
```

```console
Every 0.1s: sudo ./ptdump/ptdump 146251                                                                                                     cyberia: Sun Dec 17 18:30:59 2023

virt addr              physical addr            size          perms        present        swapped       path
0x55d44a4f7000         0x226547000                4096        r--p         true           false         /home/vilr0i/Projects/repos/linux-experiments/demand_paging/singlemap
0x55d44a4f8000         0x1f6c40000                4096        r-xp         true           false         /home/vilr0i/Projects/repos/linux-experiments/demand_paging/singlemap
0x55d44a4f9000         0x11e0ed000                4096        r--p         true           false         /home/vilr0i/Projects/repos/linux-experiments/demand_paging/singlemap
0x55d44a4fa000         0x275020000                4096        r--p         true           false         /home/vilr0i/Projects/repos/linux-experiments/demand_paging/singlemap
0x55d44a4fb000         0x1dff1d000                4096        rw-p         true           false         /home/vilr0i/Projects/repos/linux-experiments/demand_paging/singlemap
0x55d44a84f000         0x37c94c000              135168        rw-p         true           false         [heap]
0x7f348c547000         0x34d030000               12288        rw-p         true           false
0x7f348c54a000         0x32c80a000              155648        r--p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c570000         0x3bee76000             1396736        r-xp         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c6c5000                 0x0              339968        r--p        false           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c718000         0x364260000               16384        r--p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c71c000         0x287822000                8192        rw-p         true           false         /usr/lib/x86_64-linux-gnu/libc.so.6
0x7f348c71e000         0x251470000               53248        rw-p         true           false
0x7f348c73e000         0x1f0a06000                4096        rwxp         true           false         <---- We have a physical page!
0x7f348c73f000         0x201642000                8192        rw-p         true           false
0x7f348c741000         0x3bee36000                4096        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c742000         0x274923000              151552        r-xp         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c767000         0x2b320c000               40960        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c771000         0x1a78c5000                8192        r--p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7f348c773000         0x1e5fbd000                8192        rw-p         true           false         /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
0x7ffde154e000                 0x0              135168        rw-p        false           false         [stack]
0x7ffde15f4000                 0x0               16384        r--p        false           false         [vvar]
0x7ffde15f8000         0x330a0b000                8192        r-xp         true           false         [vdso]

```


## Experiment 2: Multiple Pages
The steps are the same with experiment 1. 

However, ```multimap``` requests multiple pages, and accesses each of them when a user presses enter.

While the effects are the same, I feel like watching pages successively being popluated is kind of fun(?).

The commands used are slightly differnt. 

```console
$ ./multimap
```

```console
$ watch -n 0.1 "sudo ./ptudmp/ptdump -g [pid] | awk '/virt|rwx/{print}'"
```

We will be only matching for pages with rwx (and the banner), since the output of `-g` is very large. 

A sample output after a few accesses are as follows:

```console
pid: 219540

Allocated page at 0x7f03141a6000
Allocated page at 0x7f03141a5000
Allocated page at 0x7f03141a4000
Allocated page at 0x7f03141a3000
Allocated page at 0x7f03141a2000

data written to 0x7f03141a6000

data written to 0x7f03141a5000

data written to 0x7f03141a4000

```

```console
Every 0.1s: sudo ./ptdump/ptdump -g 219540 | awk '/virt|rwx/{print}'                                                                        cyberia: Sun Dec 17 19:04:41 2023

virt addr              physical addr            size          perms        present        swapped       path
0x7f03141a2000                 0x0                4096        rwxp        false           false
0x7f03141a3000                 0x0                4096        rwxp        false           false
0x7f03141a4000         0x1c9b89000                4096        rwxp         true           false
0x7f03141a5000         0x318603000                4096        rwxp         true           false
0x7f03141a6000         0x28664d000                4096        rwxp         true           false
```
