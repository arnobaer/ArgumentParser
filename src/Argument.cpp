#include "argparse/Argument.hpp"

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

static const std::string FLAG_PREFIXES {"-+"};

Argument::Argument() = default;

Argument::Argument(const std::string& name)
{
    add_name(name);
}

Argument::~Argument() = default;

Argument& Argument::add_name(const std::string& name)
{
    assert(name.size());

    // Auto set meta name
    if (metavar_.empty())
    {
        metavar_ = flag_basename(name);
    }

    if (names_.size())
    {
        if (is_flag_ != flag_test(name))
            throw std::runtime_error("invalid name for flag/arg");
    }
    else
    {
        is_flag_ = flag_test(name);
        if (not is_flag_) set_nargs(1);
    }
    names_.emplace_back(name);
    return *this;
}

const std::vector<std::string>& Argument::names() const
{
    return names_;
}

Argument& Argument::set_metavar(const std::string& name)
{
    assert(name.size());
    metavar_ = name;
    return *this;
}

const std::string& Argument::metavar() const
{
    return metavar_;
}

Argument& Argument::set_required(bool required)
{
    is_required_ = required;
    return *this;
}

bool Argument::is_required() const
{
    return is_required_;
}

Argument& Argument::set_nargs(const size_t n)
{
    nargs_ = n;
    return *this;
}

size_t Argument::nargs() const
{
    return nargs_;
}

Argument& Argument::set_count(const size_t n)
{
    count_ = n;
    return *this;
}

size_t Argument::count() const
{
    return count_;
}

Argument& Argument::set_help(const std::string& text)
{
    help_ = text;
    return *this;
}

const std::string& Argument::help() const
{
    return help_;
}

bool Argument::is_flag() const
{
    return is_flag_;
}

// Static helpers

bool Argument::flag_test(const std::string& arg)
{
    return arg.find_first_of(FLAG_PREFIXES) == 0;
}

std::string Argument::flag_basename(const std::string& arg)
{
    return arg.substr(arg.find_first_not_of(FLAG_PREFIXES));
}

} // namespace argparse
