# Setup

Setup is a project initialisation program for CMake, inspired by the Rust language's cargo and gcc/g++ written in C++.

## Why

I got tired of rewriting very similar CMake files and decided to automate the process so that I could get to the actual programming faster.

## Example

Creating a new project from scratch

`setup new -n name -I /include -L /lib -l lib_name_1 lib_name_2`

Initialising an alreay existing project

`setup init -n name -I /include -L /lib -l lib_name_1 lib_name_2`

## Building

>[!NOTE]
>This project uses std::filesystem, which is only available from C++17 onwards. Ensure your compiler supports C++17.
>Some MinGW and gcc/g++ 8 compilers can have some trouble compiling with std::filesystem, even with C++17 support.

CMake

`cmake -B build`