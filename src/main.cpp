#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

/* TODO: 
1. switch to using <filesystem> instead of using the windows API, only getSourceFiles needs to be updated 
2. general cleanup of the code
3. add comments to the code
*/

#if defined(_WIN32) || defined(_WIN64)
    // #include <fileapi.h> NOTE: does not need to be added already available via <Windows.h>
    #include <Windows.h> 
    #define createDirectory CreateDirectoryA
    #define getSourceDirectory getSourceDirectoryWindows
#elif defined(__linux__)
    #include <dirent.h>
    #define getSourceDirectory getSourceDirectoryLinux
#endif

#include "nlohmann/json.hpp"
#include "options.h"

std::filesystem::path getSourceDirectoryWindows();
std::filesystem::path getSourceDirectoryLinux();
bool directoryExists(const char* path);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "[ERROR] No command provided\n";
        std::cout << "\tUsage: setup <command> [options]\n";
        std::exit(EXIT_FAILURE);
    }

    // Yes I know hardcoding the commands is bad, but theres only two commands so its fine
    if (strcmp(argv[1], "new") && strcmp(argv[1], "init"))
    {
        std::cout << "[ERROR] Command not recognised: " << argv[1] << "\n";
        std::cout << "\tUsage: setup <command> [options]\n";
        std::exit(EXIT_FAILURE);
    }

    ProgramContext context;
    context.working_directory = std::filesystem::current_path();
    context.source_directory = getSourceDirectory();

    parseOptions(context, argc, argv);
    context.project_name = context.options["NAME"][0]; 

    if (!strcmp(argv[1], "new"))
    {
        std::cout << "[INFO] Creating new project: " << context.project_name << "\n";
        context.working_directory /= context.project_name;

        std::cout << "[INFO] Creating project directory: " << context.working_directory << "\n";

        if (!std::filesystem::is_directory(context.working_directory))
        {
            int directory_created = std::filesystem::create_directory(context.working_directory);
            if (!directory_created)
            {
                std::cout << "Failed to create directory " << context.working_directory << "\n";
            }
        } 
    }

    context.working_directory /= "src";

    std::cout << "[INFO] Checking for src directory\n";
    if (!std::filesystem::is_directory(context.working_directory))
    {
        std::cout << "[INFO] No src directory found, creating a new src directory\n";
        int directory_created = std::filesystem::create_directory(context.working_directory);
        if (!directory_created)
        {
            std::cout << GetLastError() << "\n";
        }
    }

    if (!strcmp(argv[1], "new"))
    {
        std::string cpp_file_content = 
        "#include <iostream>\n\n"
        "int main(int argc, char* argv[])\n"
        "{\n"
            "\tstd::cout << \"Hello World!\\n\";"
            "\n\n"
            "\treturn 0;\n"
        "}";

        std::ofstream cpp_file = std::ofstream(context.working_directory / "main.cpp", std::ofstream::binary);

        cpp_file.write(cpp_file_content.c_str(), cpp_file_content.length());
        cpp_file.close();
    } 

    std::cout << "[INFO] Getting source files\n";
    getSourceFiles(context);
    
    context.working_directory.remove_filename(); 
    
    std::cout << "[INFO] Generating CMakeLists.txt\n";
    createCMakeListstxt(context);
    
    std::cout << "[INFO] Project created successfully\n";
    return 0;
}

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

std::filesystem::path getSourceDirectoryLinux()
{
    std::filesystem::path path = std::filesystem::canonical("/proc/self/exe");

    return path;
}

bool directoryExists(const char* path)
{
    DWORD result = GetFileAttributesA(path);

    if (result == INVALID_FILE_ATTRIBUTES)
        return false;

    if (result & FILE_ATTRIBUTE_DIRECTORY)
        return true;

    return false;
}