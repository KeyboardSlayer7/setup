#include <iostream>
#include <fstream>

#include "commands.h"

void newProject(ProgramContext& context)
{
    std::cout << "[INFO] Creating new project: " << context.project_name << "\n";
    context.working_directory /= context.project_name;

    std::cout << "[INFO] Creating project directory: " << context.working_directory << "\n";

    if (!std::filesystem::is_directory(context.working_directory))
    {
        int directory_created = std::filesystem::create_directory(context.working_directory);
        if (!directory_created)
        {
            std::cout << "[ERROR] Failed to create directory " << context.working_directory << "\n";
        }
    }

    initProject(context);
}

void initProject(ProgramContext& context)
{
    context.working_directory /= "src";

    std::cout << "[INFO] Checking for src directory\n";
    if (!std::filesystem::is_directory(context.working_directory))
    {
        std::cout << "[INFO] No src directory found, creating a new src directory\n";
        int directory_created = std::filesystem::create_directory(context.working_directory);
        if (!directory_created)
        {
            std::cout << "[ERROR] Failed to create directory" << context.working_directory << "\n";
        }

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
}