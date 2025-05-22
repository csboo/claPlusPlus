#include "Parser.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Arg.hpp"
#include "utils.hpp"

void ClapParser::parse(const int& argc, char* argv[]) {
    const std::string& raw_program_name = argv[0];
#ifdef _WIN32
    std::string parsed_name = raw_program_name.substr(raw_program_name.find_last_of('\\') + 1);
    parsed_name.erase(parsed_name.size() - 4);
#else
    std::string parsed_name = raw_program_name.substr(raw_program_name.find_last_of('/') + 1);
#endif
    this->program_name_ = parsed_name;
    std::vector<std::string> args(argv + 1, argv + argc);

    this->apply_defaults();
    this->check_env();
    this->parse_cli_args(args);  // parse from cli (argc, argv)

    // Validate all arguments that need values received them
    for (const auto& arg : args_) {
        // std::cerr << arg << "\n\n\n";
        if (arg.get__is_required() && !arg.has_value()) {
            throw std::runtime_error("argument '" + arg.get__name() + "' is required");
        }
    }
}

void ClapParser::add_arg(const Arg& arg) { args_.emplace_back(arg); }

void ClapParser::parse_cli_args(std::vector<std::string>& args) {
    for (size_t i = 0; i < args.size(); ++i) {
        std::string token = args.at(i);

        // TODO this could be better with string view contains?
        if (token == "--help" || token == "-h") {
            print_help();
            exit(0);
        }

        // solve --opt="value" stuff
        ClapParser::analyze_token(token, i, args);


        auto* arg = ok_or_throw_str(ClapParser::find_arg(*this, token), "unknown option: " + quote(token));

        if (!arg->get__is_flag()) {
            ClapParser::parse_value_for_non_flag(arg, i, args);
        } else {
            ClapParser::parse_value_for_flag(arg, i, args);
        }
    }
}

void ClapParser::parse_value_for_non_flag(Arg* arg, size_t& cli_index, const std::vector<std::string>& args) {
    if (cli_index + 1 < args.size() && !is_option(args.at(cli_index + 1))) {
        if (arg->get__accepts_many()) {
            std::string value;
            while (cli_index + 1 < args.size() && !is_option(args.at(cli_index + 1))) {
                value += args.at(cli_index + 1) + ' ';
                cli_index++;
            }
            arg->set__value(value);
        } else {
            arg->set__value(args.at(cli_index + 1));
            cli_index++;  // Skip the value in the next iteration
        }
    } else {
        throw std::runtime_error("option '" + arg->get__name() + "' requires a value but none was provided");
    }
}

void ClapParser::parse_value_for_flag(Arg* arg, size_t& cli_index, const std::vector<std::string>& args) {
    if (cli_index + 1 < args.size() && !is_option(args.at(cli_index + 1))) {
        if (args.at(cli_index + 1) == "true" || args.at(cli_index + 1) == "1") {
            arg->set__value("1");
            cli_index++;
        } else if (args.at(cli_index + 1) == "false" || args.at(cli_index + 1) == "0") {
            arg->set__value("0");
            cli_index++;
        } else {
            throw std::runtime_error("boolean option " + quote(arg->get__name()) + " strictly takes: true|false|1|0 (got: " + args.at(cli_index + 1) + ")");
        }
    } else {
        arg->set__value("1");
    }
}

void ClapParser::analyze_token(std::string& token, size_t& cli_index, std::vector<std::string>& args) {
    if (token.contains('=')) {
        std::cerr << "'=' found, separating token ( ";
        const auto middle = token.find('=');

        std::string token_name = token.substr(0, middle);
        std::string token_value = token.substr(middle + 1);
        if (token_value.empty()) {
            throw std::runtime_error("value not specified after '='");
        }

        args.at(cli_index) = token_value;
        cli_index--;
        
        token = token_name;
    } else {
        std::cerr << "ending token analysis, left alone\n";
    }
}

void ClapParser::check_env() {
    for (auto& arg : args_) {
        if (arg.get__auto_env()) {
            std::string env_name = this->program_name_ + '_' + arg.get__name();
            to_upper(env_name);
            auto* value_from_env = std::getenv(env_name.c_str());
            if (value_from_env != nullptr) {
                arg.set__value(value_from_env);
            }
        }
        if (arg.has_env()) {
            auto* value_from_env = std::getenv(arg.get__env_name().c_str());
            if (value_from_env != nullptr) {
                arg.set__value(value_from_env);
            }
        }
    }
};

bool ClapParser::is_option(const std::string& token) {
    return token.starts_with("--") || (token.starts_with('-') && token.size() > 1);
}

bool ClapParser::is_long_option(const std::string& token) { return token.starts_with("--"); }

bool ClapParser::is_short_option(const std::string& token) {
    return token.starts_with("-") && token.size() > 1 && token.at(1) != '-';
}

void ClapParser::print_help() const {
    std::cout << "Usage: " << this->program_name_ << " [OPTIONS]";
    std::cout << "\n\nOptions:\n";

    for (const auto& arg : args_) {
        arg.get__short_name().empty() ? std::cout << "      " : std::cout << "  -" << arg.get__short_name() << ", ";
        std::cout << "--" << arg.get__long_name();
        std::cout << "\t" << arg.get__help();
        if (arg.has_default()) {
            std::cout << " (default: " << arg.get__default_value() << ")";
        }
        if (arg.has_env()) {
            std::cout << " [env: " << arg.get__env_name() << "]";
        }
        if (arg.get__auto_env()) {
            std::string env_name = this->program_name_ + '_' + arg.get__name();
            to_upper(env_name);
            std::cout << " [def.env: " << env_name << "]";
        }
        std::cout << "\n";
    }
    std::cout << "  ";
    std::cout << "-h" << ", ";
    std::cout << "--help";
    std::cout << "\t" << "Prints this help message";
    std::cout << "\n";
}

// Helper methods
std::optional<Arg*> ClapParser::find_arg(ClapParser& parser, const std::string& arg_name) {
    auto it = std::ranges::find_if(parser.args_, [&](Arg& arg) {
        return ("--" + arg.get__long_name() == arg_name || "-" + arg.get__short_name() == arg_name);
    });

    if (it == parser.args_.end()) {
        return std::nullopt;
    }
    return &(*it);
}

void ClapParser::apply_defaults() {
    for (auto& arg : args_) {
        if (!arg.has_value() && arg.has_default()) {
            arg.set__value(arg.get__default_value());
        }
    }
}

void ClapParser::print_parser(std::ostream& os, const ClapParser& parser, int indent) {
    print_indent(os, indent);
    os << "ClapParser {\n";

    print_indent(os, indent + 1);
    os << "program_name: \"" << parser.program_name_ << "\",\n";

    print_indent(os, indent + 1);
    os << "args: [\n";
    for (const auto& arg : parser.args_) {
        Arg::print_arg(os, arg, indent + 2);
        os << ",\n";
    }
    print_indent(os, indent + 1);
    os << "],\n";

    print_indent(os, indent + 1);
    os << "}\n";

    print_indent(os, indent);
    os << "}";
}

std::ostream& operator<<(std::ostream& os, const ClapParser& parser) {
    ClapParser::print_parser(os, parser, 0);
    return os;
}
