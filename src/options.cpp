#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <sstream>
#include <unordered_map>
#include <fstream>

#include "options.h"

std::vector<std::vector<std::string>> parseOptions(char** options, int num_options)
{ 
    std::vector<std::vector<std::string>> values;
    values.resize(NUM_VALUES);

    int index = PROJECT_NAME;
    for (int i = 2; i < num_options; i++)
    {
        if (options[i][0] == '-')
        {   
            switch (options[i][1])
            {
                case 'n': 
                    index = (int)PROJECT_NAME;
                    break;
                
                case 'L':
                    index = (int)LINK_DIRECTORIES; 
                    break;

                case 'l':
                    index = (int)LINK_LIBRARIES;
                    break;

                case 'I':
                    index = (int)INCLUDE_DIRECTORIES;
                    break;

                default:
                    break;
            }
        }
        else 
        {
            values[index].push_back(std::string(options[i]));
        }
    }  

    return values;
}

std::vector<std::string> getSourceFiles(const std::string& path)
{  
    std::vector<std::string> output;

    WIN32_FIND_DATAA file_data;
    HANDLE handle = FindFirstFileA(path.c_str(), &file_data);

    int ret;
    while ((ret = FindNextFileA(handle, &file_data)) != 0)
    {
        output.push_back(file_data.cFileName);        
    }

    return output;
}

std::string combineStrings(std::vector<std::string>& strings, uint8_t spaces)
{
    std::string output;

    if (spaces > 3)
    {
        return "";
    }
    else if (spaces == 2 || spaces == 3)
    {
        output += " ";
    }

    for (int i = 0; i < strings.size(); ++i)
    {
        if (i != strings.size() - 1)
        {
            output += strings[i] + " ";
        }
        else 
        {
            output += strings[i];
            if (spaces == 3 || spaces == 1)
            {
                output += " ";
            }
        }
    }
    
    return output;
}

std::string createCMakeListstxt(std::vector<std::vector<std::string>>& options, std::vector<std::string>& source_files)
{
    // std::ostringstream output;

    std::string cmake_template = 
        "project({{PROJECT_NAME}} LANGUAGES C CXX)\n\n"
        "set(SOURCE_DIR /src)\n\n"
        "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /bin)\n\n"
        "set(SOURCES {{SOURCE_FILES}})\n\n"
        "add_executable(${PROJECT_NAME} ${SOURCES})\n\n"
        "target_include_directories(${PROJECT_NAME} PUBLIC {{INCLUDE_DIRECTORIES}})\n\n"
        "target_link_directories(${PROJECT_NAME} PUBLIC {{LINK_DIRECTORIES}})\n\n"
        "target_link_libraries(${PROJECT_NAME} {{LINK_LIBRARIES}})\n\n";

    int option_index = PROJECT_NAME;
    bool wildcard = false;
    char previous = cmake_template[0];
    int wildcard_start = 0, wildcard_end = 0;

    for (int i = 1; i < cmake_template.length(); ++i)
    { 
        if (previous == '{' && cmake_template[i] == '{')
        { 
            wildcard_start = i - 1;
        }
        else if (previous == '}' && cmake_template[i] == '}')
        {
            wildcard = true;
            wildcard_end = i;

            // std::cout << "[" << cmake_template.substr(wildcard_start + 2, (wildcard_end - 1) - (wildcard_start + 2)) << "]\n";
        }

        if (wildcard)
        {
 
            if (cmake_template.substr(wildcard_start + 2, (wildcard_end - 1) - (wildcard_start + 2)) != "SOURCE_FILES")
            {
                std::string value = combineStrings(options[option_index], 0);
                cmake_template.replace(wildcard_start, wildcard_end - wildcard_start + 1, value);
                wildcard = false;

                if (option_index < NUM_VALUES - 1)
                {
                    option_index++;
                }
            }
            else
            {
                std::string value = "";
                for (std::string& file: source_files)
                {
                    if (file != "..")
                    {
                        value += "\n${SOURCE_DIR}/" + file;
                    }
                }

                cmake_template.replace(wildcard_start, wildcard_end - wildcard_start + 1, value);
                wildcard = false;
            }
            // wildcard = false;
        }

        previous = cmake_template[i];
    }

    // return output.str();
    return cmake_template; 
}

std::string createCmakeliststxt(std::unordered_map<std::string, std::vector<std::string>>& options, std::vector<std::string>& source_files)
{
    std::ifstream cmakelists_template("cmake.template");

    cmakelists_template.seekg(0, std::ios::end);
    size_t size = cmakelists_template.tellg();
    cmakelists_template.seekg(0, std::ios::beg);
    
    char* buffer = new char[size + 1];
    buffer[size] = '\0';

    cmakelists_template.read(buffer, size);

    std::string cmake_template = std::string(buffer);

    delete[] buffer;

    size_t start = 0, end = 0;

    while (true)
    {
        start = cmake_template.find("{{", end);
        end = cmake_template.find("}}", start);

        if (start == std::string::npos || end == std::string::npos)
        {
            break;
        }

        std::string key = cmake_template.substr(start + 2, end - start - 2);
        std::string value = combineStrings(options[key], 0);

        cmake_template.replace(start, end - start + 2, value);
    }

    return cmake_template;
}