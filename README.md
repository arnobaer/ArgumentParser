# ArgumentParser
General purpose command line argument parser for C++11.

Influenced by Python's argparse module.

## Quick example

    ArgumentParser parser;

    parser.addArgument("filename")
        .setHelp("filename to open");
    parser.addArgument("--max")
        .setNArgs(1)
        .setMetaVar("n")
        .setHelp("max characters to read);
    parser.addArgument("-v")
        .setHelp("show verbose output");

    auto args = parser.parseArgs(argc, argv);

    std::string filename = args.at("filename");
    bool verbose = args.at("v").size();
