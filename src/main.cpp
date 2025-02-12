#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

/* TODO: 
1. switch to using <filesystem> instead of using the windows API 
2. for "new" option, create a new directory with the name specified by the user with -n flag 
3. general cleanup of the code
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
    // std::filesystem::path working_directory = std::filesystem::current_path(); 
    // std::filesystem::path source_directory = getSourceDirectory();

    // char path[MAX_PATH];    
    // GetCurrentDirectoryA(MAX_PATH, path);

    // std::string working_directory = std::string(path);

    ProgramContext context;
    context.working_directory = std::filesystem::current_path();
    context.source_directory = getSourceDirectory();

    parseOptions(context, argc, argv);
    context.project_name = context.options["NAME"][0];
    
    if (strcmp(argv[1], "new") && strcmp(argv[1], "init"))
    {
        std::cout << "Command not recognised: " << argv[1] << "\n";
        std::exit(EXIT_FAILURE);
    } 

    if (!strcmp(argv[1], "new"))
    {
        std::cout << context.project_name << "\n";
        context.working_directory /= context.project_name;

        std::cout << "creating new project: " << context.working_directory << "\n";

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

    if (!std::filesystem::is_directory(context.working_directory))
    {
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

    // std::string new_path = "C:\\Users\\dhruv\\Documents\\setup\\src\\*";
    // std::vector<std::string> source_files = getSourceFiles(working_directory + "\\*");

    // for (std::string& file: source_files)
    // {
    //     std::cout << file << "\n";
    // }

    getSourceFiles(context);
    
    context.working_directory.remove_filename();
    // std::string temp_path = context.working_directory.string();
    // std::cout << "temp_path: " << temp_path << "\n";
    // context.working_directory = std::filesystem::path(temp_path.erase(temp_path.begin() + temp_path.find("src"), temp_path.end())); 
    // working_directory /= "CMakeLists.txt";
    
    createCMakeListstxt(context);

    // std::ofstream cmakelists_txt(working_directory.c_str(), std::ofstream::binary);
    // std::string version = "cmake_minimum_required(VERSION 3.24.0)\n\n";
    
    // std::cout << working_directory << "\n"; 
    // std::vector<std::vector<std::string>> options = parseOptions(argv, argc);

    // version += createCMakeListstxt(options, source_files); 

    // cmakelists_txt.write(version.c_str(), version.length());
    // cmakelists_txt.close();
    
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

    std::cout << "source directory: " << output << "\n";
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