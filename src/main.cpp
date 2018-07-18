#include "ArgumentParser.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <cassert>

int main(int argc, char* argv[])
{
    ArgumentParser parser;

    parser.addArgument("mode");
    parser.addArgument("depth");

    parser.addArgument("--file")
      .setNArgs(1)
      .setRequired(true)
      .setHelp("set file");
    parser.addArgument("-f")
      .setHelp("switch to fullscreen");
    parser.addArgument("-r")
      .setNArgs(2)
      .setHelp("set resolution <width> <height>")
      .setMetaVar("n");

    try
    {
        auto args = parser.parseArgs(argc, argv);

        for (const auto& pair: args)
          std::cout << pair.first << ": " << pair.second << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        parser.help(argv[0]);
    }

    return 0;
}
