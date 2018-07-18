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

static const std::string FLAG_PREFIXES {"-+"};

bool isFlag(const std::string& arg)
{
    return arg.find_first_of(FLAG_PREFIXES) == 0;
}

std::string basename(const std::string& arg)
{
    return arg.substr(arg.find_first_not_of(FLAG_PREFIXES));
}

Argument::Argument() = default;

Argument::Argument(const std::string& name)
{
    addName(name);
}

Argument::~Argument() = default;

Argument& Argument::addName(const std::string& name)
{
    assert(name.size());

    // Auto set meta name
    if (metaVar_.empty())
    {
        metaVar_ = basename(name);
    }

    if (names_.size())
    {
        if (isFlag_ != ::isFlag(name))
            throw std::runtime_error("invalid name for flag/arg");
    }
    else
    {
        isFlag_ = ::isFlag(name);
        if (not isFlag_) setNArgs(1);
    }
    names_.emplace_back(name);
    return *this;
}

const std::vector<std::string>& Argument::names() const
{
    return names_;
}

Argument& Argument::setMetaVar(const std::string& name)
{
    assert(name.size());
    metaVar_ = name;
    return *this;
}

const std::string& Argument::metaVar() const
{
    return metaVar_;
}

Argument& Argument::setRequired(bool required)
{
    isRequired_ = required;
    return *this;
}

bool Argument::isRequired() const
{
    return isRequired_;
}

Argument& Argument::setNArgs(const size_t n)
{
    nArgs_ = n;
    return *this;
}

size_t Argument::nArgs() const
{
    return nArgs_;
}

Argument& Argument::setCount(const size_t n)
{
    count_ = n;
    return *this;
}

size_t Argument::count() const
{
    return count_;
}

Argument& Argument::setHelp(const std::string& text)
{
    help_ = text;
    return *this;
}

const std::string& Argument::help() const
{
    return help_;
}
