#pragma once
#include "Arg.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class ClapParser {
public:
    void add_arg(const Arg& arg);
    void parse(int argc, char* argv[]);
    void print_help() const;

    template<typename T>
    T get(const std::string& name) const;

private:
    std::vector<Arg> args_;
    std::unordered_map<std::string, std::any> values_;
    std::string program_name_;

    // Helper methods
    const Arg* find_option(const std::string& name) const;
    std::vector<Arg> get_positional_args() const;
    void apply_defaults();
    
    void parse_options(const std::vector<std::string>& args);
    void parse_positional_args(const std::vector<std::string>& args);
    void check_required_args();
    void handle_missing_positional(const Arg& arg);
    
    size_t handle_long_option(const std::string& token, const std::vector<std::string>& args, size_t i);
    size_t handle_short_option(const std::string& token, const std::vector<std::string>& args, size_t i);
    size_t handle_option_with_value(const Arg* arg, const std::vector<std::string>& args, size_t i, const std::string& token);
    
    static bool is_option(const std::string& token);
    static bool is_long_option(const std::string& token);
    static bool is_short_option(const std::string& token);
};

template<typename T>
T ClapParser::get(const std::string& name) const {
    auto it = values_.find(name);
    if (it == values_.end()) throw std::runtime_error("Argument not found: " + name);
    try {
        return std::any_cast<T>(it->second);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("Type mismatch for argument: " + name);
    }
}
