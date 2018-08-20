#ifndef argparse_Argument_hpp
#define argparse_Argument_hpp

#include <string>
#include <vector>

namespace argparse {

class Argument
{
public:
    Argument();
    Argument(const std::string& name);
    virtual ~Argument();

    Argument& add_name(const std::string& name);
    const std::vector<std::string>& names() const;

    Argument& set_metavar(const std::string& name);
    const std::string& metavar() const;

    Argument& set_required(bool required);
    bool is_required() const;

    Argument& set_nargs(const size_t n);
    size_t nargs() const;

    Argument& set_count(const size_t n);
    size_t count() const;

    Argument& set_help(const std::string& text);
    const std::string& help() const;

    bool is_flag() const;

    static bool flag_test(const std::string& arg);
    static std::string flag_basename(const std::string& arg);

protected:
    std::vector<std::string> names_;
    bool is_flag_ = false;
    std::string metavar_;
    bool is_required_ = false;
    size_t nargs_ = 0;
    size_t count_ = 1;
    std::string help_;
};

} // namespace argparse

#endif
