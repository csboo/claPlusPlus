#include "./include/Parser.hpp"
#include "include/Arg.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    ClapParser parser;

    parser.add_arg(Arg("input")
                       .short_name("i")
                       .required(true)
                       .long_name("input")
                       .help("Input file path")
                       .type<std::string>());

    parser.add_arg(Arg("verbose")
                       .short_name("v")
                       .long_name("verbose")
                       .help("Enable verbose output")
                       .takes_value(false)
                       .type<bool>());

    parser.add_arg(Arg("output")
                       .short_name("o")
                       .long_name("output")
                       .help("Output file path")
                       .default_value("out.txt")
                       .type<std::string>());

    try {
        parser.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n\n";
        parser.print_help();
        return 1;
    }

    parser.print_help();

    std::string input = parser.get<std::string>("input");
    bool verbose = parser.get<bool>("verbose");
    std::string output = parser.get<std::string>("output");

    std::cout << "Input: " << input << "\n";
    std::cout << "Output: " << output << "\n";
    std::cout << "Verbose: " << std::boolalpha << verbose << "\n";

    return 0;
}
