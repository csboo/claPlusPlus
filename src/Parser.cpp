#include "Parser.hpp"
#include "Arg.hpp"
#include "utils.hpp"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_set>

void ClapParser::parse(const int& argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    std::unordered_set<std::string> args_with_values;

    this->apply_defaults();
    this->check_env();
    this->parse_options(args);
    // this->parse_positional_args(args);

    // Validate all arguments that need values received them
    for (const auto& arg : this->args_) {
        if (arg.get__takes_value() && args_with_values.count(arg.get__name()) == 0) {
            if (arg.get__is_required() && !arg.has_default()) {
                throw std::runtime_error("argument '" + arg.get__name() + "' requires a value");
            }
        }
    }

    this->check_required_args();
}

void ClapParser::add_arg(const Arg& arg) { this->args_.push_back(arg); }

void ClapParser::parse_options(const std::vector<std::string>& args) {
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& token = args[i];

        if (ClapParser::is_long_option(token)) {
            i = this->handle_long_option(token, args, i);
        } else if (ClapParser::is_short_option(token)) {
            i = this->handle_short_option(token, args, i);
        } else {
            // Positional arguments are handled separately
            break;
        }
    }
}

void ClapParser::check_env() {
    for (auto& arg : this->args_) {
        if (arg.auto_env_) {
            std::string env_name = PROGRAM_NAME() + '_' + arg.get__name();
            std::transform(env_name.begin(), env_name.end(), env_name.begin(), [](const unsigned char& c) { return std::toupper(c); });
            auto value_from_env = std::getenv(env_name.c_str());
            if (value_from_env) {
                this->values_[arg.get__name()] = value_from_env;
            }
        }
        if (arg.has_env() && arg.value_.has_value()) {
            this->values_[arg.get__name()] = arg.value_.value();
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

void ClapParser::check_required_args() const {
    for (const auto& arg : this->args_) {
        if (arg.get__is_required() && this->values_.find(arg.get__name()) == this->values_.end()) {
            throw std::runtime_error("missing required argument: " + arg.get__name());
        }
    }
}

size_t ClapParser::handle_long_option(const std::string& token, const std::vector<std::string>& args, size_t i) {
    std::string opt_name = token.substr(2);
    if (opt_name == "help") {
        this->print_help();
        exit(0);
    }
    const Arg* arg = this->find_option(opt_name);
    if (arg == nullptr) {
        throw std::runtime_error("unknown option: " + token);
    }

    if (arg->get__takes_value()) {
        i = this->handle_option_with_value(arg, args, i, token);
    } else {
        this->values_[arg->get__name()] = true; // Boolean flag
    }

    return i;
}

size_t ClapParser::handle_short_option(const std::string& token, const std::vector<std::string>& args, size_t i) {
    std::string opt_name = token.substr(1);
    if (opt_name == "h") {
        this->print_help();
        exit(0);
    }
    const Arg* arg = this->find_option(opt_name);
    if (arg == nullptr) {
        throw std::runtime_error("unknown option: " + token);
    }

    if (arg->get__takes_value()) {
        i = this->handle_option_with_value(arg, args, i, token);
    } else {
        this->values_[arg->get__name()] = true; // Boolean flag
    }

    return i;
}

size_t ClapParser::handle_option_with_value(const Arg* arg, const std::vector<std::string>& args, size_t i,
                                            const std::string& token) {
    if (i + 1 < args.size() && !is_option(args[i + 1])) {
        // Use next argument as value
        this->values_[arg->get__name()] = std::string(args[i + 1]);
        return i + 1; // Skip the value in the next iteration
    }
    if (arg->has_default()) {
        // Use default value
        this->values_[arg->get__name()] = std::string(arg->get__default_value());
    } else {
        throw std::runtime_error("option '" + token + "' requires a value but none was provided");
    }

    return i;
}

void ClapParser::handle_missing_positional(const Arg& arg) {
    if (arg.get__is_required()) {
        throw std::runtime_error("missing required positional argument: " + arg.get__name());
    }
    if (arg.has_default()) {
        this->values_[arg.get__name()] = std::string(arg.get__default_value());
    }
}

  inline  bool  ClapParser::is_option(const std::string& token) {
      return token.substr(0, 2) == "--" || (token[0] == '-' && token.size() > 1);
  }
  
  inline bool ClapParser::is_long_option(const std::string& token) { return token.substr(0, 2) == "--"; }
  
  inline bool ClapParser::is_short_option(const std::string& token) {
      return token[0] == '-' && token.size() > 1 && token[1] != '-';
  }

void ClapParser::print_help() const {
    std::cout << "Usage: " << PROGRAM_NAME() << " [OPTIONS]";
    auto positionals = this->get_positional_args();
    for (const auto& pos : positionals) {
        std::cout << " [" << pos.get__name() << "]";
    }
    std::cout << "\n\nOptions:\n";

    for (const auto& arg : this->args_) {
        arg.get__short_name().empty()? std::cout << "      " : std::cout << "  -" << arg.get__short_name() << ", "; 
        std::cout << "--" << arg.get__long_name();
        std::cout << "\t" << arg.get__help();
        if (arg.has_default()) {
            std::cout << " (default: " << arg.get__default_value() << ")";
        }
        if (arg.has_env()) {
            std::cout << " [env: " << arg.get__env_name() << "]";
        }
        if (arg.get__auto_env()) {
            std::cout << " [def.env: " << arg.get__auto_env_name() << "]";
        }
        std::cout << "\n";
    }
    std::cout << "  ";
    std::cout << "-h" << ", ";
    std::cout << "--help";
    std::cout << "\t" << "Prints this help message";
    std::cout << "\n";

    if (!positionals.empty()) {
        std::cout << "\nPositional arguments:\n";
        for (const auto& pos : positionals) {
            std::cout << "  " << pos.get__name() << "\t" << pos.get__help();
            if (pos.has_default())
                std::cout << " (default: " << pos.get__default_value() << ")";
            std::cout << "\n";
        }
    }
}

// Helper methods
const Arg* ClapParser::find_option(const std::string& name) const {
    for (const auto& arg : this->args_) {
        if (arg.get__long_name() == name || arg.get__short_name() == name) {
            return &arg;
        }
    }
    return nullptr;
}

std::vector<Arg> ClapParser::get_positional_args() const {
    std::vector<Arg> positional;
    for (const auto& arg : this->args_) {
        if (arg.get__short_name().empty() && arg.get__long_name().empty()) {
            positional.push_back(arg);
        }
    }
    return positional;
}

void ClapParser::apply_defaults() {
    for (const auto& arg : this->args_) {
        if (this->values_.find(arg.get__name()) == this->values_.end() && arg.has_default()) {
            this->values_[arg.get__name()] = std::string(arg.get__default_value());
        }
    }
}

bool ClapParser::has(const std::string& name) const { return values_.find(name) != values_.end(); }

std::ostream& operator<<(std::ostream& os, const ClapParser& parser) {
    os << "ClapParser {\n";
    os << "  program_name: \"" << PROGRAM_NAME() << "\",\n";

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
