#include "../include/Parser.hpp"
#include "Arg.hpp"
#include <cctype>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <string>
#include <unordered_set>

void ClapParser::parse(int argc, char* argv[]) {
    program_name_ = argv[0];
    std::vector<std::string> args(argv + 1, argv + argc);
    std::unordered_set<std::string> args_with_values;

    apply_defaults();
    check_env();
    parse_options(args);
    // parse_positional_args(args);

    // Validate all arguments that need values received them
    for (const auto& arg : args_) {
        if (arg.takes_value() && args_with_values.count(arg.name()) == 0) {
            if (arg.is_required() && !arg.has_default()) {
                throw std::runtime_error("Argument '" + arg.name() + "' requires a value");
            }
        }
    }

    check_required_args();
}

void ClapParser::add_arg(const Arg& arg) { args_.push_back(arg); }

void ClapParser::parse_options(const std::vector<std::string>& args) {
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& token = args[i];

        if (is_long_option(token)) {
            i = handle_long_option(token, args, i);
        } else if (is_short_option(token)) {
            i = handle_short_option(token, args, i);
        } else {
            // Positional arguments are handled separately
            break;
        }
    }
}

void ClapParser::check_env() {
    for (auto& arg : args_) {
        if (arg.has_env() && arg.value_.has_value()) {
            values_[arg.name()] = arg.value_.value();
        }
    }
};

// void ClapParser::parse_positional_args(const std::vector<std::string>& args) {
//     std::vector<std::string> positional_args;

//     // Collect positional arguments (tokens not starting with '-')
//     for (const auto& token : args) {
//         if (!is_option(token)) {
//             positional_args.push_back(token);
//         }
//     }

//     // Assign positional arguments to their respective slots
//     auto positional_specs = get_positional_args();
//     for (size_t j = 0; j < positional_specs.size(); ++j) {
//         if (j < positional_args.size()) {
//             values_[positional_specs[j].name()] = positional_specs[j].convert(positional_args[j]);
//         } else {
//             handle_missing_positional(positional_specs[j]);
//         }
//     }
// }

void ClapParser::check_required_args() {
    for (const auto& arg : args_) {
        if (arg.is_required() && values_.find(arg.name()) == values_.end()) {
            throw std::runtime_error("missing required argument: " + arg.name());
        }
    }
}

size_t ClapParser::handle_long_option(const std::string& token, const std::vector<std::string>& args, size_t i) {
    std::string opt_name = token.substr(2);
    if (opt_name == "help") {
        print_help();
        exit(0);
    }
    const Arg* arg = find_option(opt_name);
    if (arg == nullptr) {
        throw std::runtime_error("Unknown option: " + token);
    }

    if (arg->takes_value()) {
        i = handle_option_with_value(arg, args, i, token);
    } else {
        values_[arg->name()] = true; // Boolean flag
    }

    return i;
}

size_t ClapParser::handle_short_option(const std::string& token, const std::vector<std::string>& args, size_t i) {
    std::string opt_name = token.substr(1);
    if (opt_name == "h") {
        print_help();
        exit(0);
    }
    const Arg* arg = find_option(opt_name);
    if (arg == nullptr) {
        throw std::runtime_error("unknown option: " + token);
    }

    if (arg->takes_value()) {
        i = handle_option_with_value(arg, args, i, token);
    } else {
        values_[arg->name()] = true; // Boolean flag
    }

    return i;
}

size_t ClapParser::handle_option_with_value(const Arg* arg, const std::vector<std::string>& args, size_t i,
                                            const std::string& token) {
    if (i + 1 < args.size() && !is_option(args[i + 1])) {
        // Use next argument as value
        values_[arg->name()] = std::string(args[i + 1]);
        return i + 1; // Skip the value in the next iteration
    }
    if (arg->has_default()) {
        // Use default value
        values_[arg->name()] = std::string(arg->default_value());
    } else {
        throw std::runtime_error("Option '" + token + "' requires a value but none was provided");
    }

    return i;
}

void ClapParser::handle_missing_positional(const Arg& arg) {
    if (arg.is_required()) {
        throw std::runtime_error("Missing required positional argument: " + arg.name());
    }
    if (arg.has_default()) {
        values_[arg.name()] = std::string(arg.default_value());
    }
}

  inline bool ClapParser::is_option(const std::string& token) const {
      return token.substr(0, 2) == "--" || (token[0] == '-' && token.size() > 1);
  }
  
  inline bool ClapParser::is_long_option(const std::string& token) const { return token.substr(0, 2) == "--"; }
  
  inline bool ClapParser::is_short_option(const std::string& token) const {
      return token[0] == '-' && token.size() > 1 && token[1] != '-';
  }

void ClapParser::print_help() const {
    std::cout << "Usage: " << program_name_ << " [OPTIONS]";
    auto positionals = get_positional_args();
    for (const auto& pos : positionals) {
        std::cout << " [" << pos.name() << "]";
    }
    std::cout << "\n\nOptions:\n";

    for (const auto& arg : args_) {
        if (!arg.short_name().empty()) {
            std::cout << "  ";
            std::cout << "-" << arg.short_name() << ", ";
            std::cout << "--" << arg.long_name();
            std::cout << "\t" << arg.help();
            if (arg.has_default())
                std::cout << " (default: " << arg.default_value() << ")";
            std::cout << "\n";
        } else {
            std::cout << "      ";
            std::cout << "--" << arg.long_name();
            std::cout << "\t" << arg.help();
            if (arg.has_default())
                std::cout << " (default: " << arg.default_value() << ")";
            std::cout << "\n";
        }
    }
    std::cout << "  ";
    std::cout << "-h" << ", ";
    std::cout << "--help";
    std::cout << "\t" << "Prints this help message";
    std::cout << "\n";

    if (!positionals.empty()) {
        std::cout << "\nPositional arguments:\n";
        for (const auto& pos : positionals) {
            std::cout << "  " << pos.name() << "\t" << pos.help();
            if (pos.has_default())
                std::cout << " (default: " << pos.default_value() << ")";
            std::cout << "\n";
        }
    }
}

// Helper methods
const Arg* ClapParser::find_option(const std::string& name) const {
    for (const auto& arg : args_) {
        if (arg.long_name() == name || arg.short_name() == name) {
            return &arg;
        }
    }
    return nullptr;
}

std::vector<Arg> ClapParser::get_positional_args() const {
    std::vector<Arg> positional;
    for (const auto& arg : args_) {
        if (arg.short_name().empty() && arg.long_name().empty()) {
            positional.push_back(arg);
        }
    }
    return positional;
}

void ClapParser::apply_defaults() {
    for (const auto& arg : args_) {
        if (values_.find(arg.name()) == values_.end() && arg.has_default()) {
            values_[arg.name()] = std::string(arg.default_value());
        }
    }
}

bool ClapParser::has(const std::string& name) const { return values_.find(name) != values_.end(); }

std::ostream& operator<<(std::ostream& os, const ClapParser& parser) {
    os << "ClapParser {\n";
    os << "  program_name: \"" << parser.program_name_ << "\",\n";

    os << "  args: [\n";
    for (const auto& arg : parser.args_) {
        os << "    " << arg << ",\n";
    }
    os << "  ],\n";

    os << "  values: {\n";
    for (const auto& [key, val] : parser.values_) {
        os << "    \"" << key << "\": \"" << val << "\",\n";
    }
    os << "  }\n";

    os << "}";
    return os;
}
