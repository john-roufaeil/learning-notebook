# Operating Systems
> A program (SW) **managing the HW**

> Acts as an intermediary between the user apps and the HW

> A resource manager managing cpu time, memory, storage, I/O devices, processes (programs)

## Table of Contents
- [Need for OS](#need-for-os)
    - [Notes](#notes)
- [Computing Systems](#computing-systems)
- [Build Process](#build-process)
- [Hardware](#hardware)
    - [Memory](#memory)
    - [CPU](#cpu)
- [Kernel](#kernel)
- [System Types](#system-types)
    - [Multiprocessor Systems (Tightly Coupled)](#1-multiprocessor-systems-tighlty-coupled)
    - [Loosely Coupled/Distributed Systems](#2-loosely-coupleddistributed-systems)
    - [Clustered Systems](#3-clustered-systems)
    - [System Types Summary](#system-types-summary)
- [Network](#network)
    - [Bi-Directional Communication](#bi-directional-communication)


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
- Time sharing = each process takes a fixed amount of time then the next process takes the same fixed amount of time.
- App in cpu called process or job

An app leaves CPU if:
- Its time sharing slice ends
- Requested IO operation
- Execution ended

---

- Simple batch system: only one app is running at a time , cpu is idle if IO request
- Multiprogramming: 

## System Types

- **Parallelism**: Multiple tasks (or parts of a task) truly run at the same time on different CPUs or cores.
- **Concurrency**: Multiple tasks appear to run at the same time, but the CPU switches between them quickly (time-sharing).

### 1. Multiprocessor Systems (Tighlty Coupled)

**Tightly coupled system**: processors share memory and a clock; communication  happens through shared memory
- Advantages:
    - Increased throughput (more work done in parallel)
    - Economical (more cores cheaper than whole machines)
    - Increased reliability (if one processor fails, others can continue)

### 2. Loosely Coupled/Distributed Systems
**Loosely Coupled System**: each processor has its own memory; processors communicate together through communication busses
- **Distributed System**: system connected in a network
- Advantages:
    - Resources sharing across multiple systems
    - Communication speed up (load sharing)... distributes work among nodes
    - Reliability (failure of one node doesn't stop the system)

### 3. Clustered Systems
**Clustered System**: two or more computers (nodes) work together and share the same storage or resources.
- A type of distributed system, typically designed for high availability (HA) and scalability.
- The main goal is to provide high reliability and improved availability, if one node fails, another can take over.
- Asymmetric Clustering:
    - Only one server actively runs the application(s).
    - The other servers stay on standby, ready to take over if the active one fails.
    - Common in failover systems for critical applications.
- Symmetric Clustering:
    - All servers (N hosts) run the application(s) simultaneously.
    - This allows for load balancing and better resource utilization since all nodes share the workload.

### System Types Summary

|System Type|Shared Memory|Shared Clock|Communication|Example|
|-|-|-|-|-|
|Tightly Coupled (Multiprocessor)|Yes|Yes|Shared memory|SMP|
|Loosely Coupled (Distributed)|No|No|Network|Microservices, cloud ndoes|
|Clustered Systems|Partial (shared storage)|No|Network|Databaser or HA clusters|

## Network

- A server is a softwarwe
- Client is anything that requests anything from the server: browser/app/prog lang
- Most common servers: web server (Apache, Nginx, ... receives HTTP requests)
- REST API is an intermediary
- Web server to the Database server is a client
- Web Server to the client is a server

### Protocols

- Web pages -> HTTP/HTTPS
- MongoDB   -> Mongodb
- A server is a softwarwe

### Connection Types on OS

![connection types on OS](OSConnectionTypes.png)

- API + Internet = web service
- All web service is API, not all API is web service
- System call is an API that OS created for an app to access the HW
- Client always initiates the converstaion

### Webservice Types

- REST API
- GraphQL
- SOAP
- gRPC
- mqtt?

### Bi-directional Communication

#### 1. Websocket

- bi-directional communication between client and server... it is a layer added to an API
- software added on API
- Developer writes and handles the websocket
- Heavy and always opened

#### 2. Pub-Sub Technique

- MQTT and many messengers works this way
- The publisher sends data to a broker, not directly to subscribers.
- The subscriber receives data from the broker, not directly from publishers.
- Both sides are independent, they don't know each other.
- Keep-alive messages are used by subscribers to confirm connection status.
- If a subscriber reconnects, the broker resends missed or recent data.

|||
|---|---|
|Topic|A topic that can be subscribed to or published to|
|Data| The content that gets shared|
|Publisher| The client that sends the data|
|Subscriber| The client that receives the data|
|Broker| The "server" that connects the received data to the subscribed client on a topic|

![pub-sub](pubsub.png)

Webhook is a concept used for notifications
