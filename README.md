\# Custom Dynamic Memory Allocator Implementation in C



\## Domain \& Overview

This project implements a Custom Dynamic Memory Allocator written entirely in ANSI/C99 standard C\[span\_0](start\_span)\[span\_0](end\_span)\[span\_1](start\_span)\[span\_1](end\_span). It simulates operating system heap allocation inside a pre-allocated contiguous memory arena (`uint8\_t heap\_pool\[64 \* 1024]`) without relying on standard library allocation routines after initialization\[span\_2](start\_span)\[span\_2](end\_span).



\## Features

\- \*\*Core API\*\*: Custom implementations of `my\_malloc()`, `my\_free()`, and `my\_realloc()`\[span\_3](start\_span)\[span\_3](end\_span).

\- \*\*Explicit Doubly Linked List\*\*: Metadata boundary headers tracking block sizes, allocation states, and pointers\[span\_4](start\_span)\[span\_4](end\_span)\[span\_5](start\_span)\[span\_5](end\_span).

\- \*\*Allocation Strategies\*\*: Supports \*\*First-Fit\*\*, \*\*Best-Fit\*\*, and \*\*Worst-Fit\*\* strategies\[span\_6](start\_span)\[span\_6](end\_span).

\- \*\*Immediate Coalescing\*\*: Automatically merges adjacent free blocks in $O(1)$ time complexity during deallocation (`my\_free()`) to combat external fragmentation\[span\_7](start\_span)\[span\_7](end\_span)\[span\_8](start\_span)\[span\_8](end\_span).

\- \*\*Memory Alignment\*\*: 8-byte alignment enforcement to comply with processor architectures\[span\_9](start\_span)\[span\_9](end\_span).

\- \*\*Heap Visualizer\*\*: Built-in inspection tool (`heap\_dump()`) reporting active block metrics and fragmentation in real time\[span\_10](start\_span)\[span\_10](end\_span).



\## Build \& Execution Guide

1\. Ensure you have GCC and Make installed.

2\. Compile the project files:

&#x20;  ```bash

&#x20;  gcc main.c allocator.c -o allocator\_demo.exe

