#include "argparse/ArgumentParser.hpp"
#include "argparse/Argument.hpp"
#include "argparse/toolbox.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <cassert>

namespace argparse {

ArgumentParser::ArgumentParser() = default;

Argument& ArgumentParser::add_argument(const std::string& name)
{
    Argument argument(name);
    arguments_.emplace_back(argument);
    return arguments_.back();
}

std::map<std::string, std::string> ArgumentParser::parse_args(int argc, char* argv[]) const
{
    std::list<std::string> args(argv, argv + argc);

    std::string execName = args.front();
    args.pop_front();

    std::map<std::string, std::string> name_space;
    for (auto argument: arguments_)
    {
        if (not argument.is_flag())
            continue;

        const auto& name = argument.names().at(0);

        size_t count = 0;
        while (std::find(args.begin(), args.end(), name) != args.end())
        {
            if (++count > argument.count())
            {
                std::ostringstream oss;
                oss << execName << ": error: too many arguments of type: " << name;
                throw std::runtime_error(oss.str());
            }

            auto it = std::find(args.begin(), args.end(), name);
            if (it == args.end())
            {
                if (argument.is_required())
                {
                    std::ostringstream oss;
                    oss << execName << ": error: missing mandatory argument: " << name;
                    throw std::runtime_error(oss.str());
                }
                name_space[name] = "<None>";
            }
            else
            {
                name_space[name] = "<True>";
                it = args.erase(it);
                for (size_t i = 0, max = argument.nargs(); i < max; ++i)
                {
                    if (i == 0)
                        name_space[name] += " [";
                    if (it == args.end() or Argument::flag_test(*it))
                    {
                        std::ostringstream oss;
                        oss << execName << ": error:  argument " << name << ": expected " << max << " argument(s)";
                        throw std::runtime_error(oss.str());
                    }
                    if (i > 0)
                        name_space[name] += ", ";
                    name_space[name] += *it;
                    it = args.erase(it);
                    if (i == max - 1)
                        name_space[name] += "]";
                }
            }
        }
    }
    for (auto argument: arguments_)
    {
        if (argument.is_flag())
            continue;

        const auto& name = argument.names().at(0);
        auto it = args.begin();
        for (size_t i = 0, max = argument.nargs(); i < max; ++i)
        {
            if (it == args.end() or Argument::flag_test(*it))
            {
                std::ostringstream oss;
                oss << execName << ": error: missing argument: " << name;
                throw std::runtime_error(oss.str());
            }
            name_space[name] += (name_space[name].size() ? ", " : "") + *it;
            it = args.erase(it);
        }
    }
    if (args.size())
    {
        std::ostringstream oss;
        oss << execName << ": error: unrecognized arguments: " << join(args, " ");
        throw std::runtime_error(oss.str());
    }
    return name_space;
}

void ArgumentParser::usage(const std::string& exec) const
{
    os << "usage: " << ::argparse::basename(exec);
    for (const auto& argument: arguments_)
    {
        if (argument.is_flag())
        {
            std::stringstream ss;
            for (size_t i = 0; i < argument.nargs(); ++i)
            {
                ss << " <";
                std::string str = argument.metavar();
                std::transform(str.begin(), str.end(),str.begin(), ::toupper);
                ss << str << ">";
            }
            if (argument.is_required())
                os << " " << argument.names().at(0) << ss.str();
            else
                os << " [" << argument.names().at(0) << ss.str() << "]";
        }
        else
            os << " <" << argument.names().at(0) << ">";
    }
    os << std::endl;
}

void ArgumentParser::help(const std::string& exec) const
{
    usage(exec);
    os << "Options:" << std::endl;
    for (const auto& argument: arguments_)
    {
        os << argument.names().at(0) << "  " << argument.help() << std::endl;
    }
}

} // namespace argparse
