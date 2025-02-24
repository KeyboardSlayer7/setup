#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <sstream>
#include <unordered_map>
#include <fstream>

#include "options.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void parseOptions(ProgramContext& context, int argc, char** argv)
{
    context.command = std::string(argv[1]);

    std::ifstream options_file(context.source_directory.string() + "\\options.json");
    json options = json::parse(options_file);

    std::string key = "";
    for (int i = 2; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            key = options[argv[i]];
        }
        else
        {
            context.options[key].push_back(argv[i]);
        }
    }
}

// void getSourceFiles(ProgramContext& context)
// {   
//     WIN32_FIND_DATAA file_data;

//     std::string path = context.working_directory.string() + "\\*";
//     HANDLE handle = FindFirstFileA((char*)path.c_str(), &file_data);

//     int ret;
//     while ((ret = FindNextFileA(handle, &file_data)) != 0)
//     {
//         if (!strcmp(file_data.cFileName, ".."))
//         {
//             continue;
//         }
//         context.options["SOURCE_FILES"].push_back(file_data.cFileName);        
//     } 
// }

void getSourceFiles(ProgramContext& context)
{
    for (auto& file : std::filesystem::directory_iterator(context.working_directory))
    {
        context.options["SOURCE_FILES"].push_back(file.path().filename().string());
    }
}

// where works as follows:
// where follows the look of the value in binary, and should only be 0, 1, 2, or 3;
// 1 - separator is added between string and at the end (01);  
// 2 - separator is added between strings and at the start (10);
// 3 - separator is added between strings and at the start and end (11);
std::string combineStrings(std::vector<std::string>& strings, std::string separator, uint8_t where)
{
    std::string output;

    if (where > 3)
    {
        return "";
    }
    else if (where == 2 || where == 3)
    {
        output += separator;
    }

    for (int i = 0; i < strings.size(); ++i)
    {
        if (i != strings.size() - 1)
        {
            output += strings[i] + separator;
        }
        else 
        {
            output += strings[i];
            if (where == 3 || where == 1)
            {
                output += separator;
            }
        }
    }
    
    return output;
}

void createCMakeListstxt(ProgramContext& context)
{
    std::ifstream cmakelists_template(context.source_directory / "cmake.template");
    std::ofstream cmakelists_txt(context.working_directory / "CMakeLists.txt", std::ofstream::binary);

    cmakelists_template.seekg(0, std::ios::end);
    size_t size = cmakelists_template.tellg();
    cmakelists_template.seekg(0, std::ios::beg);
    
    char* buffer = new char[size + 1];
    buffer[size] = '\0';

    cmakelists_template.read(buffer, size);

    std::string cmake_template = std::string(buffer);

    delete[] buffer;

    // tracks position of the wildcards
    size_t previous_end = 0;
    size_t start = 0, end = 0;

    while (true)
    {
        start = cmake_template.find("{{", end);
        end = cmake_template.find("}}", start);

        if (start == std::string::npos || end == std::string::npos)
        {
            cmakelists_txt.write(cmake_template.c_str() + previous_end, cmake_template.length() - previous_end);
            break;
        }

        std::string key = cmake_template.substr(start + 2, end - start - 2);

        // if the options for a given wildcard are empty, the wildcard stays in the CMakelists.txt
        if (context.options[key].empty())
        {
            cmakelists_txt.write(cmake_template.c_str() + previous_end, end + 2 - previous_end);

            // offset the previous_end by 2 to skip the "}}" characters
            previous_end = end + 2;
            continue;
        }

        std::string value = "";

        if (key == "SOURCE_FILES")
        {
            value = combineStrings(context.options[key], "\n\t${SOURCE_DIR}/", 2);
        }
        else
        {
            value = combineStrings(context.options[key], " ", 0);
        }
        
        // replace the wildcard with the value
        cmakelists_txt.write(cmake_template.c_str() + previous_end, start - previous_end);
        cmakelists_txt.write(value.c_str(), value.length());

        // offset the previous_end by 2 to skip the "}}" characters
        previous_end = end + 2;
    }
    
    cmakelists_template.close();
    cmakelists_txt.close();
}