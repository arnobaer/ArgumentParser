#include "ArgumentParser.hpp"
#include "Argument.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <cassert>

ArgumentParser::ArgumentParser() {}

Argument& ArgumentParser::addArgument(const std::string& name)
{
    Argument argument(name);
    arguments_.emplace_back(argument);
    return arguments_.back();
}

std::map<std::string, std::string> ArgumentParser::parseArgs(int argc, char* argv[]) const
{
    std::list<std::string> args(argv, argv + argc);

    std::string execName = args.front();
    args.pop_front();

    std::map<std::string, std::string> nameSpace;
    for (auto argument: arguments_)
    {
        if (not argument.isFlag_)
            continue;

        const auto& name = argument.names().at(0);

        size_t count = 0;
        while (std::find(args.begin(), args.end(), name) != args.end())
        {
            if (++count > argument.count())
                throw std::runtime_error("too many arguments of type " + name);

            auto it = std::find(args.begin(), args.end(), name);
            if (it == args.end())
            {
                if (argument.isRequired())
                    throw std::runtime_error("missing mandatory argument " + name);
                nameSpace[name] = "<None>";
            }
            else
            {
                nameSpace[name] = "<True>";
                it = args.erase(it);
                for (size_t i = 0, max = argument.nArgs(); i < max; ++i)
                {
                    if (i == 0)
                        nameSpace[name] += " [";
                    if (it == args.end() or ::isFlag(*it))
                        throw std::runtime_error("not enough args for argument " + name);
                    if (i > 0)
                        nameSpace[name] += ", ";
                    nameSpace[name] += *it;
                    it = args.erase(it);
                    if (i == max -1 )
                        nameSpace[name] += "]";
                }
            }
        }
    }
    for (auto argument: arguments_)
    {
        if (argument.isFlag_)
            continue;

        const auto& name = argument.names().at(0);
        auto it = args.begin();
        for (size_t i = 0, max = argument.nArgs(); i < max; ++i)
        {
            if (it == args.end() or ::isFlag(*it))
                throw std::runtime_error("missing argument " + name);
            nameSpace[name] += ", " + *it;
            it = args.erase(it);
        }
    }
    std::cout << "exec: " << execName << std::endl;
    std::cout << "left args: ";
    for (const auto& arg: args)
    {
        std::cout << "[" << arg << "]";
    }
    std::cout << std::endl;
    return nameSpace;
}

void ArgumentParser::help(const std::string& exec) const
{
    std::cout << "usage: " << exec;
    for (const auto& argument: arguments_)
    {
        if (argument.isFlag_)
        {
            std::stringstream ss;
            for (size_t i = 0; i < argument.nArgs(); ++i)
            {
                ss << " <";
                std::string str = argument.metaVar();
                std::transform(str.begin(), str.end(),str.begin(), ::toupper);
                ss << str << ">";
            }
            if (argument.isRequired())
                std::cout << " " << argument.names().at(0) << ss.str();
            else
                std::cout << " [" << argument.names().at(0) << ss.str() << "]";
        }
        else
            std::cout << " <" << argument.names().at(0) << ">";
    }
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    for (const auto& argument: arguments_)
    {
        std::cout << argument.names().at(0) << "  " << argument.help() << std::endl;
    }
}
