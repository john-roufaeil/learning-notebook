# Scheduling

## Scheduling Aglorithms

- priority: smaller number bigger priority -> could lead to starvation... solutin = aging
- first come first serve: best if smallest processes came first
- shortest job first (SJF): optimal but impossible to apply... in preemptive: Shortest Remaining Time First (SRTF)
- round robin

In general a cpu works with multiple algorithms

## Scheduler Types

1. Preemptive: incoming higher priority kicks previous
2. Non-preemptive: incoming higher priority doesn't affect current task - cannot be time-sharing

CPU can behave in both preemptive and non-preemptive ways because the overall CPU scheduling is determined by a combination of factors, and modern operating systems use a hybrid approach. The scheduler can use preemptive scheduling for certain events, like when a high-priority process arrives, while using non-preemptive scheduling for others, such as when a process voluntarily releases the CPU. 

## Dispatcher

The software that will assign the CPU the task that the scheduler decided and do the context-switching

Dispatch latency: time taken by dispatcher to stop one process and start another running (context-switching)

## Scheduling Criteria

- **CPU Utilization**
- **Throughput**
- **Turnaround time**: total time a process lives from loading in RAM till termination
- **Waiting time**: amount of time a process has been waiting in the ready queue until chosen, could be ready and waiting multiple times, we calculate the total time in queue
- **Response time**: time passed since loading first time into RAM, to entering CPU first time

-> Waiting time >= Response time

^ CPU Utilization, Throughput
v Turnaround, Waiting Time, Response Time, Biggest Response  Time 

## Deadlock

arises if 4 conditions hold simultaneously

1. Mutual exclusion: only one process at a time can use a resource.

2. Hold and wait: a process holding at least one resource is waiting to acquire additional resources held by other processes

3. No preemption: a resource can be released only voluntarily by the process holding it, after the process has completed its task.

4. Circular wait

### Solution of deadlocks

- Kill all processes having deadlock
- Kill process by process till solved