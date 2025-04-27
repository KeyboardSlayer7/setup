#include <filesystem>

#include "directories.h"

#if defined(_WIN32) || defined(_WIN64)
std::filesystem::path getSourceDirectoryWindows()
{
    char buffer[MAX_PATH];

    int length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    
    char* path = new char[length];
    memcpy(path, buffer, length);

    std::filesystem::path output(path);
    output.remove_filename();

    // std::cout << "source directory: " << output << "\n";
    delete[] path;

    return output;
}
#endif

std::filesystem::path getSourceDirectoryLinux()
{
    std::filesystem::path path = std::filesystem::canonical("/proc/self/exe");

    return path;
}

#if defined(_WIN32) || defined(_WIN64)
bool directoryExists(const char* path)
{
    DWORD result = GetFileAttributesA(path);

    if (result == INVALID_FILE_ATTRIBUTES)
        return false;

    if (result & FILE_ATTRIBUTE_DIRECTORY)
        return true;

    return false;
}
#endif
