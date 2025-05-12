#include "Parser.hpp"
#include "Arg.hpp"
#include "utils.hpp"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>

void ClapParser::parse(const int& argc, char* argv[]) {
    const std::string& raw_program_name = argv[0];
#ifdef _WIN32
    raw_program_name.substr(raw_program_name.find_last_of('\\') + 1);
    if (std::string_view(raw_program_name).ends_with(".exe")) {
        raw_program_name.erase(raw_program_name.size() - 4);
    }
    this->program_name_ = raw_program_name;
#else
    this->program_name_ = raw_program_name.substr(raw_program_name.find_last_of('/') + 1);
#endif
    std::vector<std::string> args(argv + 1, argv + argc);

    this->apply_defaults();
    this->check_env();
    this->parse_options(args); // parse from cli (argc, argv)
    // parse_positional_args(args);

    // Validate all arguments that need values received them
    for (const auto& arg : args_) {
        // std::cerr << arg << "\n\n\n";
        if (arg.get__is_required() && !arg.has_value()) {
            throw std::runtime_error("argument '" + arg.get__name() + "' is required");
        }
    }
}

void ClapParser::add_arg(const Arg& arg) { args_.emplace_back(arg); }

void ClapParser::parse_options(const std::vector<std::string>& args) {
    for (size_t i = 0; i < args.size(); ++i) {
        const auto& token = args.at(i);

        if (token == "--help" || token == "-h") {
            print_help();
            exit(0);
        }

        auto arg = ok_or_throw_str(ClapParser::find_arg(*this, token), "unknown option: \'" + token);
        if (!arg->get__is_flag()) {
            if (i + 1 < args.size() && !is_option(args[i + 1])) {
                arg->set__value(args.at(i + 1));
                i++; // Skip the value in the next iteration
            }  else {
                throw std::runtime_error("option '" + token + "' requires a value but none was provided");
            }
        } else {
            arg->set__value("1");
        }
    }
}

void ClapParser::check_env() {
    for (auto& arg : args_) {
        if (arg.get__auto_env()) {
            std::string env_name = this->program_name_ + '_' + arg.get__name();
            std::transform(env_name.begin(), env_name.end(), env_name.begin(), [](const unsigned char& c) { return std::toupper(c); });
            auto value_from_env = std::getenv(env_name.c_str());
            if (value_from_env) {
                arg.set__value(value_from_env);
            }
        }
        if (arg.has_env()) {
            auto value_from_env = std::getenv(arg.get__env_name().c_str());
            if (value_from_env) {
                arg.set__value(value_from_env);
            }
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

void ClapParser::handle_missing_positional(const Arg& arg) {
    if (arg.get__is_required()) {
        throw std::runtime_error("missing required positional argument: " + arg.get__name());
    }
    if (arg.has_default()) {
        values_[arg.get__name()] = std::string(arg.get__default_value());
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
    std::cout << "Usage: " << this->program_name_ << " [OPTIONS]";
    auto positionals = get_positional_args();
    for (const auto& pos : positionals) {
        std::cout << " [" << pos.get__name() << "]";
    }
    std::cout << "\n\nOptions:\n";

    for (const auto& arg : args_) {
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
            std::string env_name = this->program_name_ + '_' + arg.get__name();
            std::transform(env_name.begin(), env_name.end(), env_name.begin(), [](const unsigned char& c) { return std::toupper(c); });
            std::cout << " [def.env: " << env_name << "]";
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
std::optional<Arg*> ClapParser::find_arg(ClapParser& parser, const std::string& arg_name) {
    auto it = std::find_if(parser.args_.begin(), parser.args_.end(), [&](Arg& arg) { 
        return ( "--" + arg.get__long_name() == arg_name || "-" + arg.get__short_name() == arg_name );
    });

    if (it == parser.args_.end()) {
        return std::nullopt;
    }
    return &(*it);
}

std::vector<Arg> ClapParser::get_positional_args() const {
    std::vector<Arg> positional;
    for (const auto& arg : args_) {
        if (arg.get__short_name().empty() && arg.get__long_name().empty()) {
            positional.push_back(arg);
        }
    }
    return positional;
}

void ClapParser::apply_defaults() {
    for (auto& arg : args_) {
        if (!arg.has_value() && arg.has_default()) {
            arg.set__value(arg.get__default_value());
        }
    }
}

void ClapParser::print_parser(std::ostream& os, const ClapParser& parser, int indent) {
    print_indent(os, indent); os << "ClapParser {\n";

    print_indent(os, indent + 1); os << "program_name: \"" << parser.program_name_ << "\",\n";

    print_indent(os, indent + 1); os << "args: [\n";
    for (const auto& arg : parser.args_) {
        Arg::print_arg(os, arg, indent + 2);
        os << ",\n";
    }
    print_indent(os, indent + 1); os << "],\n";

    print_indent(os, indent + 1); os << "values: {\n";
    for (const auto& [key, val] : parser.values_) {
        print_indent(os, indent + 2); os << "\"" << key << "\": \"" << val << "\",\n";
    }
    print_indent(os, indent + 1); os << "}\n";

    print_indent(os, indent); os << "}";
}

std::ostream& operator<<(std::ostream& os, const ClapParser& parser) {
    ClapParser::print_parser(os, parser, 0);
    return os;
}
