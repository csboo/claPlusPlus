#pragma once
#include <ostream>
#include <iostream>
#include <complex>
#include <functional>
#include <optional>
#include <string>

class ClapParser;

class Arg {
  public:
    explicit Arg(std::string name);

    Arg& short_name(const std::string& s);
    Arg& long_name(const std::string& l);
    Arg& help(const std::string& help);
    Arg& required(bool is_required);
    Arg& takes_value(bool takes);
    Arg& default_value(const std::string& default_val);
    Arg& from_env(const char* env_var_name);
    Arg& try_env();
    void set_try_env_name(const std::string& s);

    // Getters
    [[nodiscard]] const std::string& short_name() const;
    [[nodiscard]] const std::string& long_name() const;
    [[nodiscard]] const std::string& help() const;
    [[nodiscard]] const std::string& default_value() const;
    [[nodiscard]] bool is_required() const;
    [[nodiscard]] bool requires_value() const;

    friend std::ostream& operator<<(std::ostream& os, const Arg& arg);

  private:
    friend class ClapParser;

    std::string name_;
    std::string short_;
    std::string long_;
    std::string help_;
    bool required_;
    bool takes_value_;
    bool has_env_;
    std::string env_name_;
    bool try_env_;
    std::string try_env_name_;
    std::string default_;
    std::optional<std::string> value_;

    [[nodiscard]] bool has_default() const;
    [[nodiscard]] bool has_env() const;
    [[nodiscard]] bool takes_value() const;
    [[nodiscard]] const std::string& name() const;
};
