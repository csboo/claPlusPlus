#pragma once

#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Arg.hpp"
#include "Parsables.hpp"
#include "utils.hpp"

class ClapParser {
   public:
    void add_arg(const Arg& arg);
    void parse(const int& argc, char* argv[]);
    void print_help() const;

    template <typename T>
        requires Parseable<T>
    std::optional<T> get_one_as(const std::string& name) {
        Arg* arg = ok_or(ClapParser::find_arg(*this, "--" + name), [] { return std::nullopt; });
        
        return Parse<T>::parse(
            ok_or_throw_str(arg->get__value(), "value for option: " + quote(arg->get__name()) + " is missing")
        );
    }

    static void print_parser(std::ostream& os, const ClapParser& parser, int indent);
    friend std::ostream& operator<<(std::ostream& os, const ClapParser& parser);

   private:
    std::vector<Arg> args_;
    std::string program_name_;

    // Helper methods
    static bool is_option(const std::string& token);
    static bool is_long_option(const std::string& token);
    static bool is_short_option(const std::string& token);
    static std::optional<Arg*> find_arg(ClapParser& parser, const std::string& name);
    void apply_defaults();

    void parse_cli_args(std::vector<std::string>& args);
    void check_env();
    void parse_positional_args(const std::vector<std::string>& args);
    static void parse_value_for_non_flag(Arg* arg, size_t& cli_index, const std::vector<std::string>& args);
    static void parse_value_for_flag(Arg* arg, size_t& cli_index, const std::vector<std::string>& args);
    static void analyze_token(std::string& token, size_t& cli_index, std::vector<std::string>& args);
};
