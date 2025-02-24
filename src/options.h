#ifndef OPTIONS_H
#define OPTIONS_H

#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

enum Values
{
    PROJECT_NAME,
    INCLUDE_DIRECTORIES,
    LINK_DIRECTORIES,
    LINK_LIBRARIES,
    NUM_VALUES
};

struct ProgramContext
{
    std::string command, project_name;
    std::unordered_map<std::string, std::vector<std::string>> options;
    std::filesystem::path working_directory, source_directory;
};

void parseOptions(ProgramContext& context, int argc, char** argv);
void getSourceFiles(ProgramContext& context);
std::string combineStrings(std::vector<std::string>& strings, std::string separator, uint8_t where);
void createCMakeListstxt(ProgramContext& context);

#endif