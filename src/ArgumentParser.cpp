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

class ArgumentParser::impl
{
public:
    using value_type = std::vector<Argument>;
    using args_type = std::list<std::string>;

    std::string progname;
    value_type arguments;
    std::ostream& os = std::cout;

    Argument& add_argument(const std::string& name)
    {
        arguments.emplace_back(name);
        return arguments.back();
    }

    Namespace parse_args(int argc, char* argv[])
    {
        args_type args = args_type(argv, argv + argc);

        if (progname.empty())
        {
            progname = args.front();
        }
        args.pop_front();

        Namespace ns;
        for (auto argument: arguments)
        {
            if (not argument.is_flag())
                continue;

            const auto& name = argument.names().at(0);
            const auto& id = argument.id();

            size_t count = 0;
            while (std::find(args.begin(), args.end(), name) != args.end())
            {
                if (++count > argument.count())
                {
                    std::ostringstream oss;
                    oss << progname << ": error: too many arguments of type: " << name;
                    throw std::runtime_error(oss.str());
                }

                auto it = std::find(args.begin(), args.end(), name);
                if (it == args.end())
                {
                    if (argument.is_required())
                    {
                        std::ostringstream oss;
                        oss << progname << ": error: missing mandatory argument: " << name;
                        throw std::runtime_error(oss.str());
                    }
                    ns[id] = "<None>";
                }
                else
                {
                    ns[id] = "<True>";
                    it = args.erase(it);
                    for (size_t i = 0, max = argument.nargs(); i < max; ++i)
                    {
                        if (i == 0)
                            ns[id] += " [";
                        if (it == args.end() or Argument::flag_test(*it))
                        {
                            std::ostringstream oss;
                            oss << progname << ": error:  argument " << join(argument.names(), "/");
                            oss << ": expected " << max << " argument" << (max > 1 ? "s" : "");
                            throw std::runtime_error(oss.str());
                        }
                        if (i > 0)
                            ns[id] += "|";
                        ns[id] += *it;
                        it = args.erase(it);
                        if (i == max - 1)
                            ns[id] += "]";
                    }
                }
            }
        }
        for (auto argument: arguments)
        {
            if (argument.is_flag())
                continue;

            const auto& name = argument.names().at(0);
            const auto& id = argument.id();

            auto it = args.begin();
            for (size_t i = 0, max = argument.nargs(); i < max; ++i)
            {
                if (it == args.end() or Argument::flag_test(*it))
                {
                    std::ostringstream oss;
                    oss << progname << ": error: missing argument: " << name;
                    throw std::runtime_error(oss.str());
                }
                ns[name] += (ns[name].size() ? ", " : "") + *it;
                it = args.erase(it);
            }
        }
        for (auto argument: arguments)
        {
            if (argument.is_required())
            {
                if (not ns.count(argument.id()))
                {
                    std::ostringstream oss;
                    oss << progname << ": error: argument " << join(argument.names(), "/") << " is required";
                    throw std::runtime_error(oss.str());
                }
            }
        }
        if (args.size())
        {
            std::ostringstream oss;
            oss << progname << ": error: unrecognized arguments: " << join(args, " ");
            throw std::runtime_error(oss.str());
        }
        return ns;
    }

    void usage() const
    {
        os << "usage: " << ::argparse::basename(progname);
        for (const auto& argument: arguments)
        {
            if (argument.is_flag())
            {
                std::stringstream ss;
                for (size_t i = 0; i < argument.nargs(); ++i)
                {
                    ss << " <";
                    ss << argument.metavar() << ">";
                }
                if (argument.is_required())
                    os << " " << argument.names().at(0) << ss.str();
                else
                    os << " [" << argument.names().at(0) << ss.str() << "]";
            }
        }
        for (const auto& argument: arguments)
        {
            if (not argument.is_flag())
            {
                os << " <" << argument.names().at(0) << ">";
            }
        }
        os << std::endl;
    }

    void options() const
    {
        os << "Options:" << std::endl;
        for (const auto& argument: arguments)
        {
            os << join(argument.names(), ", ") << "  " << argument.help() << std::endl;
        }
    }
};

ArgumentParser::ArgumentParser()
: pimpl(new impl())
{
}

ArgumentParser::ArgumentParser(const std::string& progname)
: pimpl(new impl())
{
  pimpl->progname = progname;
}

ArgumentParser::~ArgumentParser() = default;

Argument& ArgumentParser::add_argument(const std::string& name)
{
    return pimpl->add_argument(name);
}

Namespace ArgumentParser::parse_args(int argc, char* argv[]) const
{
    return pimpl->parse_args(argc, argv);
}

void ArgumentParser::usage() const
{
    pimpl->usage();
}

void ArgumentParser::help() const
{
    pimpl->usage();
    pimpl->options();
}

} // namespace argparse
