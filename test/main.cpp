#include "argparse/ArgumentParser.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    argparse::ArgumentParser parser;

    parser.add_argument("mode");
    parser.add_argument("depth");

    parser.add_argument("--file")
        .set_metavar("filename")
        .set_nargs(1)
        .set_required(true)
        .set_help("set file");
    parser.add_argument("-f")
        .set_help("switch to fullscreen");
    parser.add_argument("-r")
        .set_nargs(2)
        .set_help("set resolution <width> <height>")
        .set_metavar("n");
    parser.add_argument("-h")
        .add_name("--help")
        .set_help("show this help message and exit");

    try
    {
        auto args = parser.parse_args(argc, argv);

        if (args.count("help"))
        {
            parser.help();
            return 0;
        }

        for (const auto& pair: args)
          std::cout << pair.first << "=" << pair.second << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        parser.usage();
        //parser.help(argv[0]);
    }

    return 0;
}
