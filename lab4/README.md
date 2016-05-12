# CS350-Program1

# Lab 4
Generate list of memeory references simulating a number of running processes

input parameters
- `pid <int a m l p>` main process id (process we are testing)
    - `int` process id
    - `a` int specifying address space size in pages
    - `m <1|2|3>` average number of memory references (see below)
    - `l <1|2|3>` for locality of reference (see below)
    - `p <1|2|3>` likelihood of locality of reference changing (see below)
- `n` 1 to 200 number of different processes
- `a <1|2|3>` average address space size of processes
    - `1` small (1-5 pages)
    - `2` medium (5-20 pages)
    - `3` large (20-50 pages)
- `m <1|2|3>` average number of memory references
    - `1` small (500 - 2000)
    - `2` medium (2000 - 5000)
    - `3` large (5000 - 10000)
- `l <1|2|3>` locality of reference
    - `1` sparse 40% same page twice in a row
    - `2` medium 70% same page twice in a row
    - `3` highly localized 90% same page twice in a row
- `p <0|1|2|3>` likely hood of a process changing phases
    - `0` zero (0% chance)
    - `1` low (10% chance)
    - `2` medium (50% chance)
    - `3` high (80% chance)

# Lab 5
Read input from lab4 program and detect page faults when memory is full
and clear frames used by process when it terminates.

input parameters
- `pid <int>` processes id to focus on for testing for testing (we are testing this process relative to all other processes in input)
- `s <int>` memory size in pages
- `r <0|1>` page replacement policy scope. 0 for global, 1 for local

Output overall page fault rate for provided process and overall page fault rate using a global or local counter-based LRU algorithm

# Program 1
## Abstract
Global page replacement policy selects victim pages on page faults from all available
pages currently in memory. Local page replacement allocates a set of frames to each
process, from which victim pages can be selected. Both algorithms select least recently
used pages as the victim. Due to the global page replacement's unbounded nature,
other process' behavior affects the behavior (page fault rate) of a certain process.
Since each process has a set of frames with which to work, the behavior of other processes
is less likely to affect it. However, local page replacement increases the number
of unused frames, since a process may not need all frames it is allocated by the 
system while at the same time increases the number of page faults for processes with
large address spaces (compared to global replacement). This causes local page replacement
policy to have lower throughput when compared to global page replacement.

## Question
How do outside processes affect the page fault rate of a certain process in a 
system using global page replacent versus local page replacement policies?

## Data Set
Run `lab 4` with the following parameters 10 times:
- main process has large address space, with few memory references and low locality
    - many surrouding processes with small address space and more frequent references
        - this increases the likelihood of the main process' pages being least recently used
        - expected result: local page replacement having fewer page faults for main process
        - `-pid 1 100 1 1 0` `-n 100` `-a 1` `-m 3` `-l 3` `-p 0`
    - few surrounding processes with small address space and equally frequent references
        - this decreases the likelihood of the main process' pages being least frequently used but doesn not eliminate it. 
        - expected result: global page replacement having less page faults than previous case, local still better
        - `-pid 1 100 1 1 0` `-n 10` `-a 1` `-m 1` `-l 3` `-p 0`
- main process has small address space with many memory references and low locality
    - many surrounding processes with small address space and frequent references
        - this increases the overall page fault rate for all processes
        - expected result: global and local page replacement will have a high page fault rate
        - `-pid 1 10 3 1 0` `-n 100` `-a 1` `-m 3` `-l 1` `-p 0`
    - many surrounding processes with small address space and frequent references
        - this increases the overall page fault rate for all processes with high locality
        - expected result: outside processes will not have much of an effect on main process
        - `-pid 1 10 3 1 0` `-n 100` `-a 1` `-m 3` `-l 3` `-p 0`

- For each case compare overall page fault rate for local page replacement to global page replacement
- For each case compare main process' page fault rate for local page replacement to global page replacement 

## Results
- Case 1:
    - Global
        - Total fault rate: 0.021985
        - Focus fault rate: 0.436520
    - Local
        - Total fault rate: 0.043325
        - Focus fault rate: 0.584738
- Case 2:
    - Global
        - Total fault rate: 0.020869
        - Focus fault rate: 0.350545
    - Local
        - Total fault rate: 0.020155
        - Focus fault rate: 0.420281
- Case 3:
    - Global
        - Total fault rate: 0.022277
        - Focus fault rate: 0.038552
    - Local
        - Total fault rate: 0.426142
        - Focus fault rate: 0.310822
- Case 4: 
    - Global
        - Total fault rate: 0.018832
        - Focus fault rate: 0.041379
    - Local
        - Total fault rate: 0.045721
        - Focus fault rate: 0.359701

## Conclusion
In all cases, global page replacement outperforms local replacement. Only when 
there is a program with a large adderess space does global replacement performance suffer.
When keeping the behavior of a program constant and decreasing the number of
surrounding processes and memory references, global overall page fault rate
remains the same, but the focus process fault rate decreases, showing that 
outside processes had an impact on a certain process' behavior. The impact, however,
was minimal.
