#include <iostream>

#include "Arg.hpp"
#include "Parser.hpp"
#include "utils.hpp"

void run(ClapParser& arg_parser);

int main(const int argc, char* argv[]) {
    ClapParser arg_parser;
    auto num1 = Arg().long_name("num1").from_env("ASDF").auto_env().is_required();
    // std::cerr << num1 << "\n";
    arg_parser.add_arg(num1);
    auto num2 = Arg().long_name("num2").short_name("N").from_env("TES").default_value("99.0");
    arg_parser.add_arg(num2);

    arg_parser.add_arg(Arg().long_name("test").is_flag());
    // arg_parser.add_arg(Arg("test").is_flag(true));

    try {
        arg_parser.parse(argc, argv);
        std::cerr << arg_parser;
        run(arg_parser);
    } catch (const std::exception& e) {
        std::cerr << "\n\n\nerror: " << e.what() << "\n\n\n";
        // arg_parser.print_help();
    }
}

void run(ClapParser& arg_parser) {
    // std::cerr << "running\n";
    auto num1 = ok_or_throw_str(arg_parser.get_one_as<int>("num1"), "num1 not defined");
    auto num2 = ok_or_throw_str(arg_parser.get_one_as<double>("num2"), "num2 not defined");

    std::cout << "num1: " << num1 << '\n';
    std::cout.precision(5);
    std::cout << std::fixed << "num2: " << num2 << '\n';

    // std::cerr << arg_parser;
}
