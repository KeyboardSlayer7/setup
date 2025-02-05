#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <vector>
#include <cstdint>

enum Values
{
    PROJECT_NAME,
    INCLUDE_DIRECTORIES,
    LINK_DIRECTORIES,
    LINK_LIBRARIES,
    NUM_VALUES
};

std::vector<std::vector<std::string>> parseOptions(char** options, int num_options);
std::vector<std::string> getSourceFiles(const std::string& path);
std::string combineStrings(std::vector<std::string>& strings, uint8_t spaces);
std::string createCMakeListstxt(std::vector<std::vector<std::string>>& options, std::vector<std::string>& source_files);

#endif