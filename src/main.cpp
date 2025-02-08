#include <iostream>
#include <fstream>
#include <string>
// #include <filesystem>

/* TODO: 
1. switch to using <filesystem> instead of using the windows API 
2. for "new" option, create a new directory with the name specified by the user with -n flag 
3. general cleanup of the code
*/

#if defined(_WIN32) || defined(_WIN64)
    // #include <fileapi.h> NOTE: does not need to be added already available via <Windows.h>
    #include <Windows.h> 
    #define createDirectory CreateDirectoryA
#elif defined(__linux__)
    #include <dirent.h>
#endif

#include "options.h"

bool directoryExists(const char* path);

int main(int argc, char* argv[])
{
    // std::filesystem::path wd = std::filesystem::current_path(); 
    
    char path[MAX_PATH];    
    GetCurrentDirectoryA(MAX_PATH, path);

    std::string working_directory = std::string(path);
    
    if (strcmp(argv[1], "new") && strcmp(argv[1], "init"))
    {
        std::cout << "Command not recognised: " << argv[1] << "\n";
        std::exit(EXIT_FAILURE);
    } 

    if (!strcmp(argv[1], "new"))
    {
        working_directory += "\\project";

        std::cout << working_directory << "\n";

        if (!directoryExists(working_directory.c_str()))
        {
            int directory_created = createDirectory(working_directory.c_str(), NULL);
            if (!directory_created)
            {
                std::cout << GetLastError() << "\n";
            }
        }

        // std::filesystem::create_directory(wd);
    }

    working_directory += "\\src";

    // if (!std::filesystem::is_directory(wd)) 
    // {
    //     std::filesystem::create_directory(wd);
    // }

    if (!directoryExists(working_directory.c_str()))
    {
        int directory_created = createDirectory(working_directory.c_str(), NULL);
        if (!directory_created)
        {
            std::cout << GetLastError() << "\n";
        }
    }

    if (!strcmp(argv[1], "new"))
    {
        std::string cpp_file_content = 
        "#include <iostream>\n\nint main(int argc, char* argv[])\n{\n\tstd::cout << \"Hello World!\\n\";\n\n\treturn 0;\n}";
        std::ofstream cpp_file = std::ofstream(working_directory + "\\main.cpp", std::ofstream::binary);

        cpp_file.write(cpp_file_content.c_str(), cpp_file_content.length());

        cpp_file.close();
    }

    // std::string new_path = "C:\\Users\\dhruv\\Documents\\setup\\src\\*";
    std::vector<std::string> source_files = getSourceFiles(working_directory + "\\*");

    // for (std::string& file: source_files)
    // {
    //     std::cout << file << "\n";
    // }   

    working_directory.erase(working_directory.end() - 3, working_directory.end());
    working_directory += "CMakeLists.txt";

    std::ofstream cmakelists_txt(working_directory.c_str(), std::ofstream::binary);
    std::string version = "cmake_minimum_required(VERSION 3.24.0)\n\n";
    
    // std::cout << working_directory << "\n"; 
    std::vector<std::vector<std::string>> options = parseOptions(argv, argc);

    version += createCMakeListstxt(options, source_files); 

    cmakelists_txt.write(version.c_str(), version.length());
    cmakelists_txt.close();
    
    return 0;
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