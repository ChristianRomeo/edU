## Introduction
This project was part of a course on theoretical computer science, algorithms and data structures I had the opportunity to follow during my bachelor degree.

The aim of the project was to create a revisit of the popular 70s "Ed" text editor with the addition of unlimited number of possible undo and redo operations.
The program is then able to identify and execute the commands according to the [specifications](https://github.com/ChristianRomeo/edU/blob/main/specifications.pdf).

It was entirely written in C11, with the help of tools like Valgrind (and Callgrind) for memory debugging and profiling, GDB for debugging, ASan (address sanitizers) for memory error detection and compiled with GCC.

The focus was on performances, both in terms of computational time and memory footprint, assessed through an automated evaluator.
