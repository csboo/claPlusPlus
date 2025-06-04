#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "arg/Arg.hpp"
#include "Parsables.hpp"
#include "arg/BaseArg.hpp"
#include "utils.hpp"

class ClapParser {
   public:
    template<typename... States>
    void add_arg(Arg<States...> arg) { args_.emplace_back(std::make_unique<Arg<States...>>(std::move(arg))); }
    void parse(const int& argc, char* argv[]);
    void print_help() const;

    template <typename T>
        requires Parseable<T>
    std::optional<T> get_one_as(const std::string& name) {
        auto *arg = ok_or_throw_str(ClapParser::find_arg(*this, name), "no option with name: " + quote(name) + " was found");
        auto value = ok_or_throw_str(arg->get__value(), "value for option: " + quote(arg->get__long_name()) + " is missing");

        return Parse<T>::parse(value);
    }

    static void print_parser(std::ostream& os, const ClapParser& parser, int indent);
    friend std::ostream& operator<<(std::ostream& os, const ClapParser& parser);

   private:
    std::vector<std::unique_ptr<BaseArg>> args_;
    std::string program_name_;

    // Helper methods
    static bool is_option(const std::string& token);
    static bool is_long_option(const std::string& token);
    static bool is_short_option(const std::string& token);
    static std::optional<BaseArg*> find_arg(ClapParser& parser, const std::string& arg_name);
    // static std::optional<BaseArg*> find_arg(ClapParser& parser, const std::string& name);
    void apply_defaults();

    void parse_cli_args(std::vector<std::string>& args);
    void check_env();
    void parse_positional_args(const std::vector<std::string>& args);
    static void parse_value_for_non_flag(BaseArg* arg, size_t& cli_index, const std::vector<std::string>& args);
    static void parse_value_for_flag(BaseArg* arg, size_t& cli_index, const std::vector<std::string>& args);
    static void analyze_token(std::string& token, size_t& cli_index, std::vector<std::string>& args);
};
