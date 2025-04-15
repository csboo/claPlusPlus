#pragma once

#include "Arg.hpp"

#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class ClapParser {
  public:
    void add_arg(const Arg& arg);
    void parse(const int& argc, char* argv[]);
    void print_help() const;

    template <typename T> std::optional<T> get_one_as(const std::string& name) const;
    bool has(const std::string& name) const;

    friend std::ostream& operator<<(std::ostream& os, const ClapParser& parser);
  private:
    std::vector<Arg> args_;
    std::unordered_map<std::string, std::string> values_;
    std::string program_name_;

    // Helper methods
    inline bool is_option(const std::string& token) const ;
    inline bool is_long_option(const std::string& token) const ;
    inline bool is_short_option(const std::string& token) const ;
    const Arg* find_option(const std::string& name) const;
    std::vector<Arg> get_positional_args() const;
    void apply_defaults();

    void parse_options(const std::vector<std::string>& args);
    void parse_positional_args(const std::vector<std::string>& args);
    void check_required_args();
    void check_env();
    void handle_missing_positional(const Arg& arg);

    size_t handle_long_option(const std::string& token, const std::vector<std::string>& args, size_t i);
    size_t handle_short_option(const std::string& token, const std::vector<std::string>& args, size_t i);
    size_t handle_option_with_value(const Arg* arg, const std::vector<std::string>& args, size_t i,
                                    const std::string& token);

};

template <typename T> std::optional<T> ClapParser::get_one_as(const std::string& name) const {
    auto it = values_.find(name);
    if (it == values_.end()) {
        return std::nullopt;
    }

    T value;
    std::istringstream(it->second) >> value;
    return value;
    // return std::any_cast<T>(it->second);
}
