#ifndef argparse_ArgumentParser_hpp
#define argparse_ArgumentParser_hpp

#include "argparse/Argument.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace argparse {

using Namespace = std::map<std::string, std::string>;

class ArgumentParser
{
public:
    ArgumentParser();
    ArgumentParser(const std::string& progname);
    ~ArgumentParser();

    Argument& add_argument(const std::string& name);

    Namespace parse_args(int argc, char* argv[]) const;

    void usage() const;
    void help() const;

private:
    class impl;
    std::unique_ptr<impl> pimpl;
};

} // namespace argparse

#endif
