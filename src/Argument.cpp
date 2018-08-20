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

Argument::Argument(const std::string& name)
{
    add_name(name);
}

Argument::~Argument() = default;

Argument& Argument::add_name(const std::string& name)
{
    assert(name.size());
    const std::string base = flag_basename(name);

    // Auto set meta name '--foo' => 'FOO'
    if (metavar_.empty())
    {
        metavar_ = base;
        std::transform(metavar_.begin(), metavar_.end(), metavar_.begin(), ::toupper);
    }

    if (names_.size())
    {
        if (is_flag_ != flag_test(name))
            throw std::runtime_error("error: invalid name for flag/arg");
    }
    else
    {
        is_flag_ = flag_test(name);
        if (not is_flag_) set_nargs(1);
    }
    names_.emplace_back(name);

    // Auto select identifier (either first short flag or first long flag)
    // '-f -F' -> 'f'
    // '-f --file' -> 'file'
    if (id_.size() < base.size())
    {
        id_ = (id_.size() < 2 ? base : id_);
    }

    return *this;
}

const std::string& Argument::id() const
{
    return id_;
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
