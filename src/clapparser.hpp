#include <iostream>
#include <unordered_map>
#include <vector>

#include "./args.hpp"

class ClapParser {
  public:
    void add_arg(const Arg& arg);
    void parse(int argc, char* argv[]);
    void print_help() const;

    template <typename T> T get(const std::string& name) const;

  private:
    std::vector<Arg> args_;
    std::unordered_map<std::string, std::any> values_;
    std::string program_name_;

    const Arg* find_option(const std::string& name) const;
    std::vector<Arg> get_positional_args() const;
    void apply_defaults();
};

void ClapParser::add_arg(const Arg& arg) { args_.push_back(arg); }

void ClapParser::parse(int argc, char* argv[]) {
    program_name_ = argc > 0 ? argv[0] : "";
    std::vector<std::string> args(argv + 1, argv + argc);
    std::vector<std::string> positional_args;

    size_t i = 0;
    while (i < args.size()) {
        std::string token = args[i];
        if (token.substr(0, 2) == "--") {
            // Handle long options (--option)
            std::string opt_name = token.substr(2);
            const Arg* arg = find_option(opt_name);
            if (!arg) {
                throw std::runtime_error("Unknown option: " + token);
            }

            if (arg->takes_value()) {
                // Check if the next argument is a valid value (not a flag)
                if (i + 1 < args.size() && args[i + 1][0] != '-') {
                    // Use next argument as value
                    ++i;
                    values_[arg->name()] = arg->convert(args[i]);
                } else {
                    // No valid value provided
                    if (arg->has_default()) {
                        values_[arg->name()] = arg->convert(arg->default_value());
                    } else {
                        throw std::runtime_error("Option '" + token + "' requires a value but none was provided");
                    }
                }
            } else {
                // Boolean flag: set to true
                values_[arg->name()] = true;
            }

        } else if (token[0] == '-') {
            // Handle short options (-o)
            std::string opt_name = token.substr(1);
            const Arg* arg = find_option(opt_name);
            if (!arg) {
                throw std::runtime_error("Unknown option: " + token);
            }

            if (arg->takes_value()) {
                // Check if the next argument is a valid value (not a flag)
                if (i + 1 < args.size() && args[i + 1][0] != '-') {
                    // Use next argument as value
                    ++i;
                    values_[arg->name()] = arg->convert(args[i]);
                } else {
                    // No valid value provided
                    if (arg->has_default()) {
                        values_[arg->name()] = arg->convert(arg->default_value());
                    } else {
                        throw std::runtime_error("Option '" + token + "' requires a value but none was provided");
                    }
                }
            } else {
                // Boolean flag: set to true
                values_[arg->name()] = true;
            }

        } else {
            // Positional argument
            positional_args.push_back(token);
        }

        ++i; // Move to next token
    }

    // Handle positional arguments
    auto positional_specs = get_positional_args();
    for (size_t j = 0; j < positional_specs.size(); ++j) {
        if (j < positional_args.size()) {
            // Assign provided positional value
            values_[positional_specs[j].name()] = positional_specs[j].convert(positional_args[j]);
        } else {
            // Handle missing positionals
            if (positional_specs[j].is_required()) {
                throw std::runtime_error("Missing required positional argument: " + positional_specs[j].name());
            } else if (positional_specs[j].has_default()) {
                values_[positional_specs[j].name()] = positional_specs[j].convert(positional_specs[j].default_value());
            }
        }
    }

    // Check required arguments
    for (const auto& arg : args_) {
        if (arg.is_required() && values_.find(arg.name()) == values_.end()) {
            throw std::runtime_error("Missing required argument: " + arg.name());
        }
    }

    // Apply defaults for options not encountered during parsing
    apply_defaults();
}

template <typename T> T ClapParser::get(const std::string& name) const {
    auto it = values_.find(name);
    if (it == values_.end())
        throw std::runtime_error("Argument not found: " + name);
    try {
        return std::any_cast<T>(it->second);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("Type mismatch for argument: " + name);
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
            values_[arg.name()] = arg.convert(arg.default_value());
        }
    }
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
