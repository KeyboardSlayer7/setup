#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "options.h"
#include "directories.h"
#include "commands.h"

void run(ProgramContext& context);
void printUsageAndExit();

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "[ERROR] No command provided\n";
        printUsageAndExit();
    }

    // Yes I know hardcoding the commands is bad, but theres only two commands so its fine
    // Might change later if I feel like adding more commands
    if (strcmp(argv[1], "new") && strcmp(argv[1], "init"))
    {
        std::cout << "[ERROR] Command not recognised: " << argv[1] << "\n";
        printUsageAndExit();
    }

    ProgramContext context;
    context.working_directory = std::filesystem::current_path();
    context.source_directory = getSourceDirectory();

    parseOptions(context, argc, argv);
    context.project_name = combineStrings(context.options["NAME"], "_", 0);
    
    run(context);    
    
    std::cout << "[INFO] Project created successfully\n";
    return 0;
}

void run(ProgramContext& context)
{
    if (context.command == "new")
        newProject(context);
    else if (context.command == "init")
        initProject(context);
}

void printUsageAndExit()
{
    std::cout << "\tUsage: setup <command> [options]\n";
    std::exit(EXIT_FAILURE);
}