# Kernel for Intel 8086 Processor

## Project Overview

This project implements a small yet functional kernel for an Intel 8086-based system. It supports multithreading with time-sharing, memory management, and process synchronization through semaphores. The kernel handles asynchronous context switching and preemption on timer and keyboard interrupts, making it suitable for embedded systems where the kernel and the user application share the same address space.

### Features

- **Multithreading**: Lightweight threads run within the user application.
- **Time-sharing**: Implemented with a preemptive scheduler.
- **Memory Management**: Custom memory allocator for efficient resource utilization.
- **Semaphores**: Support for process synchronization.
- **Context Switching**: Asynchronous context switching on interrupts from timers and keyboard.
- **Embedded System Architecture**: Single program environment with statically linked kernel and application.

## Requirements

- **Processor Architecture**: Intel 8086
- **Development Language**: Assembly (Intel 8086), C/C++
- **Execution Environment**: Intel 8086 emulator (for virtual execution)
