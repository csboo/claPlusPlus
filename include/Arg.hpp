#pragma once

#include "Macros.hpp"

#include <iostream>
#include <optional>
#include <ostream>
#include <string>

class Arg {
  public:
    Arg(std::string name);

    Arg& short_name(const std::string& short_name);
    Arg& long_name(const std::string& long_name);
    Arg& help(const std::string& help);
    Arg& required(bool is_required);
    Arg& is_flag();
    Arg& accepts_many();
    Arg& default_value(const std::string& default_val);
    Arg& from_env(const char* env_var_name);
    Arg& auto_env();

    static void print_arg(std::ostream& os, const Arg& arg, int indent);
    friend std::ostream& operator<<(std::ostream& os, const Arg& arg);

  private:
    friend class ClapParser;

    std::string name_;
    std::string short_name_;
    std::string long_name_;
    std::string help_;
    bool is_required_;
    bool is_flag_;
    bool accepts_many_;
    std::string env_name_;
    bool auto_env_;
    std::string default_value_;
    std::optional<std::string> value_;

    // ----| Getters & Setters |----
    DEFINE_GETTER_SETTER(name, std::string)
    DEFINE_GETTER_SETTER(short_name, std::string)
    DEFINE_GETTER_SETTER(long_name, std::string)
    DEFINE_GETTER_SETTER(help, std::string)
    DEFINE_GETTER_SETTER(is_required, bool)
    DEFINE_GETTER_SETTER(is_flag, bool)
    DEFINE_GETTER_SETTER(accepts_many, bool)
    DEFINE_GETTER_SETTER(env_name, std::string)
    DEFINE_GETTER_SETTER(auto_env, bool)
    DEFINE_GETTER_SETTER(default_value, std::string)
    DEFINE_GETTER_SETTER(value, std::optional<std::string>)
    
    // ----| Checkers |----
    // has_env_
    [[nodiscard]]  bool has_env() const { return !this->env_name_.empty(); }
    
    // has_default_
    [[nodiscard]]  bool has_default() const { return !this->default_value_.empty(); }

    // has_value_
    [[nodiscard]]  bool has_value() const { return this->value_.has_value(); }
};
