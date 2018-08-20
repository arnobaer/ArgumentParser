# ArgumentParser
General purpose command line argument parser for C++11.

Influenced by Python's argparse module.

## Quick example

    ArgumentParser parser;

    parser.add_argument("filename")
        .set_help("filename to open");
    parser.add_argument("--max")
        .set_nargs(1)
        .set_metavar("n")
        .set_help("max characters to read);
    parser.add_argument("-v")
        .set_help("show verbose output");

    auto args = parser.parse_args(argc, argv);

    std::string filename = args.at("filename");
    bool verbose = args.at("v").size();
