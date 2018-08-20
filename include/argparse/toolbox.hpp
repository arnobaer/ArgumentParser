#ifndef argparse_toolbox_hpp
#define argparse_toolbox_hpp

#include <sstream>
#include <string>
#include <vector>

namespace argparse {

// Join iterable to string
template<typename T>
std::string join(const T& iterable, const std::string& separator)
{
  std::ostringstream oss;
  for (const auto& item : iterable)
      oss << (oss.tellp() ? separator : std::string()) << item;
  return oss.str();
}

// Return basename from full path
std::string basename(const std::string& path)
{
#ifdef _WIN32
    const char sep = '\\';
#else
    const char sep = '/';
#endif
   size_t pos = path.rfind(sep, path.length());
   if (pos != std::string::npos)
   {
      return path.substr(pos + 1);
   }
   return path;
}

} // namespace argparse

#endif
