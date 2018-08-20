#ifndef argparse_ArgumentParser_hpp
#define argparse_ArgumentParser_hpp

#include "Argument.hpp"

#include <iostream>
#include <string>
#include <map>
#include <vector>

namespace argparse {

class ArgumentParser
{
public:
    std::vector<Argument> arguments_;

public:
    ArgumentParser();

    Argument& add_argument(const std::string& name);

    std::map<std::string, std::string> parse_args(int argc, char* argv[]) const;

    void usage(const std::string& exec) const;

    void help(const std::string& exec) const;

    std::ostream& os = std::cout;
};

} // namespace argparse

#endif
