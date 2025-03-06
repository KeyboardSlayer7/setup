# Setup

Setup is a project initialisation program for CMake, inspired by the Rust language's Cargo and gcc/g++ written in C++ using [nlohmann/json](https://github.com/nlohmann/json). 
Setup automates the process of setting up CMake files, so that you can focus on actual programming instead of repetitive boilerplate.

## Why

I got tired of rewriting very similar CMake files and decided to automate the process so that I could get to the actual programming faster.

## Key Aspect / Features (Design Goals)

- **Familiarity**: If you have ever used a terminal, especially when compiling C/C++ projects, using Setup should be a breeze due to its familiar syntax. 
- **Customisable**: Setup allows you to add new options, allowing for CMake files to be generated just the way you like. [See more here.](#customising) 
- **Beginner Friendly**: If you are new to C/C++ and/or build systems like CMake, this can be a great place to start as all the hard work is done for you. All you need to do is learn what the options do.

## Porject Status

:white_check_mark: What Works

- Creating a new project
- Initialising existing project
- CMakeLists.txt generation

:construction: In Progress

- Improved error handling
- Adding support for verbose options
- Adding a `--help` option, that works with custom commands 

:bug: Known Issues 

- Additional bytes being written at the end of generated CMakeLists.txt 

:bulb: Future Plans

- Maintainance

## Usage

Create a New Project

`setup new -n name -I /include -L /lib -l lib_name_1 lib_name_2`

Initialise an Exisiting Project

`setup init -n name -I /include -L /lib -l lib_name_1 lib_name_2`

## Building

>[!NOTE]
>This project uses std::filesystem, which is only available from C++17 onwards. Ensure your compiler supports C++17.
>Some MinGW and gcc/g++ 8 compilers can have some trouble compiling with std::filesystem, even with C++17 support.

Requirements

- A C++17 compliant compiler
- CMake.

Clone repo

`git clone https://github.com/KeyboardSlayer7/setup.git`

CMake

`cmake -B build`

`cmake --build build`

Once these step are complete the directory which holds the executable should be added to your systems PATH, making the program executable from anywhere.

## How

Setup works by reading the options in the `options.json` file and loading in the corresponding wildcards it needs to look for in the `cmake.template` file. The program then parses the options that were provided storing the values in a `std::unordered_map` where the keys are the wildcards. Once the options have been parsed the program runs through the `cmake.template` file looking for wildcards and replacing them with the values provided in the newly generated CMakeLists.txt.

## Customising

This project offers you the ability to customise how your CMake files are generated, allowing you to tailor the program to your needs.

**Adding new options**

Adding new options can be done in the `options.json` file located next to the executable. When adding a new option ensure you add the flag and the corresponding wildcard as a key-pair.

If you have added a new option to `options.json`, you can make use of your new option by modifying the `cmake.template` file. When modifying the `cmake.template` file ensure any wildcards you add match the wildcard in the `options.json` file, and also ensure wildcard in the `cmake.template` file are wrapped with a "{{" and "}}" or you may run into some trouble.

## Feedback

I've only been working with C/C++ for a few years and am still fairly new to CMake, so I'd love to hear thoughts! If you find any bugs or have any suggestions, feel free to email me at keyboardslayer7@protonmail.com. Any feedback, big or small, is greatly appreciated!