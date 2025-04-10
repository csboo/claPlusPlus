#pragma once
#include <any>
#include <functional>
#include <string>

class Arg {
  public:
    explicit Arg(std::string name);

    Arg& short_name(const std::string& s);
    Arg& long_name(const std::string& l);
    Arg& help(const std::string& help);
    Arg& required(bool is_required);
    Arg& takes_value(bool takes);
    Arg& default_value(const std::string& default_val);

    // Getters
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] const std::string& short_name() const;
    [[nodiscard]] const std::string& long_name() const;
    [[nodiscard]] const std::string& help() const;
    [[nodiscard]] bool is_required() const;
    [[nodiscard]] bool takes_value() const;
    [[nodiscard]] bool has_default() const;
    [[nodiscard]] const std::string& default_value() const;
    [[nodiscard]] std::string convert(const std::string& s) const;
    [[nodiscard]] bool requires_value() const;

  private:
    std::string name_;
    std::string short_;
    std::string long_;
    std::string help_;
    bool required_;
    bool takes_value_;
    std::string default_;
    std::function<std::any(const std::string&)> converter_;
};
