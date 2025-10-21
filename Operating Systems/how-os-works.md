# How OS Works

1. Power On
2. BIOS: check HW, change boot config
3. Bootloader[GRUB2] -> Menu of all available OS
4. Linux (Kernel)
5. Systemd/init (open default apps, GUI, etc...)

    > ```bash
    ># Run MongoDB on start
    >sudo systemctl enable mongod.service
    > ```

6.

BIOS (Basic Input/Output System), a fundamental firmware on a computer's motherboard that initializes and tests system hardware components and loads the operating system when the computer is turned on. It acts as an interface between the operating system and the hardware.

<details>
    <summary>More from ChatGPT</summary>

    Anything that will work needs to be moved to RAM

    1. When you press the power button:

        - The CPU is reset and begins execution from a fixed memory address, a location hard-coded into the CPU (for example, 0xFFFF0 on x86).
        - That address maps to the BIOS firmware, which resides in non-volatile ROM or flash memory on the motherboard.
    At this point, nothing is yet loaded into RAM — RAM isn’t even initialized yet.

    2. BIOS Execution from ROM

        - The CPU starts executing instructions directly from the ROM chip where the BIOS is stored.
        - These instructions are read-only and typically reside in a flash memory (electrically erasable programmable ROM, not traditional “hard” ROM).
        - Early instructions initialize the CPU registers, memory controller, and chipset so the system can access RAM.

    3. Shadowing BIOS to RAM (for speed)

        Once the BIOS has initialized RAM, it often copies itself (or parts of itself) from ROM into RAM. This process is called BIOS shadowing.
        Why? ROM is much slower than RAM.
        Executing BIOS code from RAM speeds up further operations (like POST routines and disk access setup).
        So yes — after initial boot, portions of the BIOS are usually shadowed (copied) into RAM and executed from there, but only after RAM has been initialized.

    4. Handoff to Bootloader
        After POST (Power-On Self-Test) and hardware initialization: The BIOS locates a bootable device. It loads the bootloader (like GRUB or Windows Boot Manager) from that device into RAM. Then transfers control to it.

</details>

## Polling & Interrupt

Polling: CPU polls the IO device each every specific duration
Interrupt: CPU waits for the IO device to interrupt

Polling is useful if the expected time is too little, instead of starting work and get interrupted without finishing any work to prevent the cost of context-switching (this is more related to ES)

## Multithreading, Sync & Async

Multithreading is having a process and dividing to several threads.
We have a main thread and multiple others. Works using concurrency (one app and time-sharing with threads) not real parallelism. If on multiple cores then it is parallelism but not multithreading.

## Interrupts

ISR (Interrupt Service R)
Interrupt Vector Table changes from device to another
The shorter the interrupt the better

## Busses

1. Address Bus
2. Data Bus
3. Control Bus

Port mapping: a bit in the cpu to specify whether to use memory bus or io memory bus

C can only communicate with RAM
Assembly can communicate with CPU Registers
Any app must use system call to access hardware

RAM is the biggest memory accessible to the CPU

![Computer Architecture](computerArchitecture.png)


