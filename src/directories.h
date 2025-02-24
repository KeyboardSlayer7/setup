#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <filesystem>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h> 
    #define createDirectory CreateDirectoryA
    #define getSourceDirectory getSourceDirectoryWindows
#elif defined(__linux__)
    #include <dirent.h>
    #define getSourceDirectory getSourceDirectoryLinux
#endif

std::filesystem::path getSourceDirectoryWindows();
std::filesystem::path getSourceDirectoryLinux();

bool directoryExists(const char* path);

#endif // DIRECTORIES_H