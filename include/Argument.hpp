#ifndef Argument_hpp
#define Argument_hpp

#include <string>
#include <vector>

bool isFlag(const std::string& arg);
std::string basename(const std::string& arg);

class Argument
{
public:
    Argument();
    Argument(const std::string& name);
    virtual ~Argument();

    Argument& addName(const std::string& name);
    const std::vector<std::string>& names() const;

    Argument& setMetaVar(const std::string& name);
    const std::string& metaVar() const;

    Argument& setRequired(bool required);
    bool isRequired() const;

    Argument& setNArgs(const size_t n);
    size_t nArgs() const;

    Argument& setCount(const size_t n);
    size_t count() const;

    Argument& setHelp(const std::string& text);
    const std::string& help() const;

    bool isFlag_ = false;

protected:
    std::vector<std::string> names_;
    std::string metaVar_;
    bool isRequired_ = false;
    size_t nArgs_ = 0;
    size_t count_ = 1;
    std::string help_;
};

#endif
