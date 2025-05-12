#pragma once

#include "Arg.hpp"
#include "utils.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class ClapParser {
  public:
    void add_arg(const Arg& arg);
    void parse(const int& argc, char* argv[]);
    void print_help() const;

    template <typename T> inline std::optional<T> get_one_as(const std::string& name) {
        Arg* arg = ok_or(ClapParser::find_arg(*this, "--" + name), []{ return std::nullopt; });

        if (auto arg_value = arg->get__value(); arg_value) {
            T value;
            std::istringstream(*arg_value) >> value;
            return value;
        }
        return std::nullopt;
    }

    static void print_parser(std::ostream& os, const ClapParser& parser, int indent);
    friend std::ostream& operator<<(std::ostream& os, const ClapParser& parser);
  private:
    std::vector<Arg> args_;
    std::unordered_map<std::string, std::string> values_;
    std::string program_name_;

    // Helper methods
    inline bool is_option(const std::string& token) const ;
    inline bool is_long_option(const std::string& token) const ;
    inline bool is_short_option(const std::string& token) const ;
    static std::optional<Arg*> find_arg(ClapParser& parser, const std::string& name);
    std::vector<Arg> get_positional_args() const;
    void apply_defaults();

    void parse_options(const std::vector<std::string>& args);
    void check_env();
    void parse_positional_args(const std::vector<std::string>& args);
    void handle_missing_positional(const Arg& arg);
};
