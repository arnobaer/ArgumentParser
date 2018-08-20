#include "argparse/ArgumentParser.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    argparse::ArgumentParser parser;

    parser.add_argument("mode");
    parser.add_argument("depth");

    parser.add_argument("--file")
      .set_nargs(1)
      .set_required(true)
      .set_help("set file");
    parser.add_argument("-f")
      .set_help("switch to fullscreen");
    parser.add_argument("-r")
      .set_nargs(2)
      .set_help("set resolution <width> <height>")
      .set_metavar("n");

    try
    {
        auto args = parser.parse_args(argc, argv);

        for (const auto& pair: args)
          std::cout << pair.first << ": " << pair.second << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        parser.usage(argv[0]);
        //parser.help(argv[0]);
    }

    return 0;
}
