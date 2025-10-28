# Operating Systems
> A program (SW) **managing the HW**

> Acts as an intermediary between the user apps and the HW

> A resource manager managing cpu time, memory, storage, I/O devices, processes (programs)

## Table of Contents
- [Need for OS](#need-for-os)
- [Computing Systems](#computing-systems)
- [Build Process](#build-process)

## Need for OS

- Each CPU has a certain set of instructions that it understands called the Instruction Set Architecture (ISA).
- Hence, the language to communicate with the CPU differs from a CPU to another.
- We would have needed to write code in machine language (then assembly) but different for each CPU.
- Writing code with C, we're able to write code for all CPU but still IO device dependent.
> The first and most important goal for OS is to fill the gap between HW and SW and make them indepedent

### Notes

A system is a combination of components each having specific function

Assembly is HW dependent

C is IO and OS dependent

Microprocessor: CPU and several supporting chips

Microcontroller: CPU on a single chip

OS job is to be an abstraction layer between HW and SW; we download apps depending on OS not CPU

SW depends on OS not HW

## Computing Systems

1. Embedded/Mobile System
2. General Purpose System

Embedded/Mobile: small hardware designed for a specific task (e.g., AC, TV, Microwave, etc...). 
It has limited resources.

TDE (Time Driven Event)

RTOS (Real-Time Operating System):
    - Hard; time is critical
    - Soft; time sensitivity is not critical, just for convenience

RTOS should have more idle time

General Purpose: any system that can connect to any IO and can do any task

Yocto project is a Linux Foundation open source project to create Linux distros for embedded and IoT SW, independent of architecture and HW

Von Neumann architecture: CPU + Memory + IO

## Build Process

- Build process: code.c -> toolchain -> .exe (machine language)
- Not all machine language is executable
- Java -> compiler -> Bytecode (machine language - runnable on any OS with JVM)
- SW -> JVM -> OS -> HW
- JVM - Java Virtual Machine
- JRE -> Java Runtime Environment
- JDK -> Java Development Kit
- to run java app only needed JRE
- to dev java app needed JDK
- Java is fully compiled and fully interpreted
- Python has multiple translators: pypy, cpython, jython, micropython, etc...
- Java write once, run everywhere
- NodeJS runtime environment

![JDK Comparison](JDK.png)

> Dynamic linking (missing ddl library), when app is run it is linked with the library on runtime. Static linking like C where the run code has the libraries available. Trade-off between speed and space. Interpreter and Compiler.

## Hardware

SOC = System on Chip

### Memory

- ROM is read-only memory for the CPU, but we can write as developers
- Hard disk is IO not memory storage / or secondary storage.
- OS reserves resources -> loads your app in RAM (instructions & data) -> then OS leaves
- OS is loaded in RAM, but not always running... (OS is responsible for assiging resources not running apps, CPU runs)
- RAM and ROM are not HW dependent, because we only need read and write
- OS = the administrator of the system
- RAM is divided into instructions and data spaces

> the less the OS interrupts, the more successful the system

### CPU

#### Components:
- Decode Circuit
- MMU (Memory Management Unit); doesn't let the developer utilize the physical addresses, we instead use virtual/logical. Only MMU and OS know the real addresses.
- Registers; program counter (el3aleh eldoor), CIR (current instruction register), ACC (accumulator), MAR (memory address register), MDR (memory data register)
- ALU

- Rust is the fastest language
-- node js, bun js

> Fetch -> Decode -> Execute -> Store

> least interrupt is more important than biggest task

## Kernel

> The one program running at all times

- The kernel must be loaded in RAM and actively scheduled on the CPU for the OS to be running.
- A program must be loaded into RAM, and its instructions must be scheduled on the CPU to be executing.
- User = Person/App/Machine/Developer/End User
- RUST, C++ -> smart pointers
- appointed time ended/process finished/went for IO process
- Time sharing = each process takes a fixed amount of time then the next process takes the same fixed amount of time.
- App in cpu called process or job

---

- Simple batch system: only one app is running at a time , cpu is idle if IO request
- Multiprogramming: 