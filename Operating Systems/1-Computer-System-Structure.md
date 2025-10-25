# Computer System Structure

Any computer system can be divided into 4 main components:
1. Hardware - provides basic computing resources
    > CPU, RAM, IO
2. Operating System - controls use of hardware among apps and users
3. Apps - define the ways that the users will use the system resources
    > Word, compilers, browser, db, games
4. Users
    > People, other computers, machines

![computer system structure](computer-system-structure.png)

## What is OS?

A program (SW) **managing the HW**; acts as an intermediary between the user apps and the HW; a resource manager: cpu time, memory, storage, I/O devices, processes (programs)


**The kernel is the one program running at all times on the computer**

## OS objectives and functions

- execute users programs
- solve its problems
- use HW efficiently

## OS System components

* Kernel
    - The core of the OS
    - Directly controls the HW (CPU, memory, disks, etc.)
    - Stays in RAM all the time
    - Runs in kernel mode, i.e., has full access to the system HW
    - Decides which program runs next (Process mgmt)
    - Allocates and frees RAM (memory mgmt)
    - Talks to I/O devices (device mgmt)
    - Reading/Writing to disk (file mgmt)
    - System calls handling

* Shell
    - The interface between the user and the kernel
    - Takes commands from user, sends to kernel, and shows results
    - Can be command line shell like bash or graphical shell like gnome or windows explorer

* System Calls

    - Functions that let programs talk to the kernel
    - It's like a door between user space and kernel space

### User Mode vs Kernel Mode

|Mode|Who runs here|Privilege level|Can access HW directly?|
|---|---|---|---|
|User Mode|Normal apps (like browsers, editors)| Low|No|
|Kernel Mode|OS kernel and device drivers| High|Yes|

- When a user program needs to access HW, it must ask the kernel through a system call.
- This separation prevents user programs from crashing or damaging the whole system.

## Types of operating systems

1. Batch OS

    - Used in old mainframes
    - No direct interaction between user and computer
    - Users submit jobs (program + data), and OS executes them one by one in batches
    - Example: Early IBM systems
    - Good for large repetitive jobs, but has no interaction during execution and long waiting times.

2. Multiprogramming OS

    - Multiple programs are loaded in memory at the same time
    - CPU switches between them when one is waiting (e.g., for IO)
    - Improves CPU utilization
    - Example: Early UNIX systems
    - Better resource use, faster processing, but needs memory mgmt and protection.

3. Multitasking OS

    - Extension of multiprogramming, but for a single user
    - Allows a single user to run multiple programs at the same time
    - CPU switches quickly between tasks, creating the illusion of parallelism
    - Example: Windows, macOS, Linux desktops.

4. Time-Sharing OS

    - Special type of multitasking system for **multiple users**
    - CPU time is divided into time slices, each user/process gets a small portion
    - After its turn, the CPU moves to the next process, giving the illusion that everyone is using the computer simultaneously.
    - If memory is full, some jobs are swapped out to disk temporarily until it’s their turn again.
    - Interactive for multiple users, efficient resource sharing, but high overhead for context switching.

5. Real-Time Operating System (RTOS)

    - Designed for time-critical systems where actions must happen within a fixed deadline.
    - Every task must finish within a specific time, or system failure occurs.
    - Examples: Airplane control systems, medical devices, robotics, automotive systems.
    - Types:
        - Hard Real-Time: Must meet deadlines (e.g., pacemaker).
        - Soft Real-Time: Occasional delays acceptable (e.g., video streaming).

## OS architecture

How OS internal components are structured and interact.

1. Monolithic Architecture

    - A monolithic kernel is an OS architecture where all core services (like process management, memory, file system, device drivers, etc.) run inside one big kernel space.
    - When a user program makes a system call, control jumps into the kernel.
    - Inside the kernel, all modules can directly communicate and share data.
    - Example: Linux, MS-DOS, Early UNIX

2. Microkernel Architecture

    - In a microkernel, only the essential functions of the OS run in kernel model (CPU scheduling, Interprocess Communication (IPC), basic memory mgmt, low level IO), everything else runs in user mode as separate processes called servers.
    - All other things (like file system, drivers, device management) run outside the kernel — in user space.
    - Example: Minix, QNX, Mach

3. Hybrid Architecture

    - Combines both monolithic and microkernel features, it keeps performance of monolithic design, but adds modularity and safety of microkernel.
    - Some services (like drivers or file systems) run in kernel mode for speed.
    - Others may run in user mode for isolation.
    - Example: Windows NT/10, macOS, Modern Linux

## System calls

> A system call is the way a program requests a service from the Operating System kernel.

- User programs run in user mode (no direct hardware access for safety).
- The kernel runs in kernel mode (full hardware control).

### How System Calls Work (Step-by-Step)

1. Your program runs in user mode.

2. It wants to perform an OS-level task (like writing to disk).

3. It makes a system call → triggers a software interrupt.

4. CPU switches to kernel mode.

5. The kernel performs the requested operation.

6. CPU switches back to user mode and returns the result.


## Multiprocessor Systems (Parallel Systems)

- Systems with more than one CPU in close communication  
- Tightly coupled system — also known as multiprocessor systems  
- Processors share memory and a clock; communication occurs through shared memory  

### Advantages
- Increased throughput  
- Economical  
- Increased reliability  
  - Graceful degradation  
  - Fail-soft systems  

## Distributed Systems

- Computation is distributed among several physical processors  
- Loosely coupled system — each processor has its own local memory  
- Processors communicate via communication lines such as high-speed buses or telephone lines  
- Requires networking infrastructure (LAN or WAN)  
- Can be client-server or peer-to-peer systems  

### Advantages
- Resource sharing  
- Computation speed-up  
- Load sharing  
- Reliability  

## Clustered Systems

- Two or more systems share storage  
- Provides high reliability  

### Types
- Asymmetric Clustering: one server runs the application(s), others are on standby  
- Symmetric Clustering: all N hosts run the application(s) simultaneously  

## Real-Time Systems

- Used as control devices in dedicated applications such as:  
  - Scientific experiments  
  - Medical imaging systems  
  - Industrial control systems  
  - Display systems  
- Have well-defined, fixed-time constraints  
- Can be Hard or Soft real-time systems  

### Hard Real-Time
- Strict time constraints; delays are unacceptable  
- Secondary storage limited or absent  
- Data stored in short-term memory or ROM  
- Not supported by general-purpose OS (conflicts with time-sharing)  

### Soft Real-Time
- Less strict timing but still requires quick responses  
- Limited use in industrial control or robotics  
- Can be integrated with time-sharing systems  
- Useful for multimedia and virtual reality applications  

## Handheld Systems

- Includes PDAs, cellular phones, and smartphones  

### Issues
- Limited memory  
- Slow processors  
- Small display screens  

---

## Computing Environments

### Traditional Computing
- PCs and servers  
- Limited remote access  

### Web-Based Computing
- Client-server and web services  
- Convenient remote access  
- Location-independent servers  

### Embedded Computing
- Very limited operating system features  
- Little or no user interface  
- May allow remote access  


---

Users want convenience, ease of use and good performance
But shared computers must keep all users happy