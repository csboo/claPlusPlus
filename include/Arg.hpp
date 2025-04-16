#pragma once

#include "utils.hpp"

#include <cerrno>
#include <ostream>
#include <iostream>
#include <optional>
#include <string>

class Arg {
  public:
    Arg(const std::string& name);

    Arg& short_name(const std::string& short_name);
    Arg& long_name(const std::string& long_name);
    Arg& help(const std::string& help);
    Arg& required(bool is_required);
    Arg& takes_value(bool takes);
    Arg& default_value(const std::string& default_val);
    Arg& from_env(const char* env_var_name);
    Arg& auto_env();

    friend std::ostream& operator<<(std::ostream& os, const Arg& arg);

  private:
    friend class ClapParser;

    std::string name_;
    std::string short_name_;
    std::string long_name_;
    std::string help_;
    bool is_required_;
    bool takes_value_;
    std::string env_name_;
    bool auto_env_;
    // std::string auto_env_name_;
    std::string default_value_;
    std::optional<std::string> value_;

    // ----| Getters & Setters |----
    // name_
    [[nodiscard]] inline const std::string& get__name() const {
      return this->name_;
    }
    inline void set__name(const std::string& name) {
      this->name_ = name;
    }

    // short_
    [[nodiscard]] inline const std::string& get__short_name() const {
      return this->short_name_;
    }
    inline void set__short_name(const std::string& short_name) {
      this->short_name_ = short_name;
    }

    // long_
    [[nodiscard]] inline const std::string& get__long_name() const {
      return this->long_name_;
    }
    inline void set__long_name(const std::string& long_name) {
      this->long_name_ = long_name;
    }

    // help_
    [[nodiscard]] inline const std::string& get__help() const {
      return this->help_;
    }
    inline void set__help(const std::string& help) {
      this->help_ = help;
    }

    // required_
    [[nodiscard]] inline bool get__is_required() const {
      return this->is_required_;
    }
    inline void set__is_required(const bool& is_required) {
      this->is_required_ = is_required;
    }

    // takes_value_
    [[nodiscard]] inline bool get__takes_value() const {
      return this->takes_value_;
    }
    inline void set__takes_value(const bool& takes_value) {
      this->takes_value_ = takes_value;
    }

    // env_name_
    [[nodiscard]] inline const std::string& get__env_name() const {
      return this->env_name_;
    }
    inline void set__env_name(const std::string& env_name) {
      this->env_name_ = env_name;
    }

    // auto_env_
    [[nodiscard]] inline bool get__auto_env() const {
      return this->auto_env_;
    }
    inline void set__auto_env(const bool& auto_env) {
      this->auto_env_ = auto_env;
    }

    // auto_env_name_
    [[nodiscard]] inline const std::string get__auto_env_name() const {
      std::string env_name = PROGRAM_NAME() + '_' + this->get__name();
      to_upper(env_name);
      return env_name;
    }

    // default_
    [[nodiscard]] inline const std::string& get__default_value() const {
      return this->default_value_;
    }
    inline void set__default_value(const std::string& default_value) {
      this->default_value_ = default_value;
    }

    // value_
    [[nodiscard]] inline const std::optional<std::string> get__value() const {
      return this->value_;
    }
    inline void set__value_(const std::string& value) {
      this->value_ = value;
    }

    // ----| Checkers |----
    // has_env_
    [[nodiscard]] inline bool has_env() const {
      return !this->env_name_.empty();
    }

    // has_default_
    [[nodiscard]] inline bool has_default() const {
      return !this->default_value_.empty();
    }

    // has_value_
    [[nodiscard]] inline bool has_value() const {
      return !this->value_.has_value();
    }

};
