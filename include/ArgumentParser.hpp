#ifndef ArgumentParser_hpp
#define ArgumentParser_hpp

#include "Argument.hpp"

#include <string>
#include <map>
#include <vector>

class ArgumentParser
{
public:
    std::vector<Argument> arguments_;

public:
    ArgumentParser();
    Argument& addArgument(const std::string& name);
    std::map<std::string, std::string> parseArgs(int argc, char* argv[]) const;

    void help(const std::string& exec) const;
};

#endif
