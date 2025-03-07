#pragma once
#include <any>
#include <functional>
#include <stdexcept>
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

    template <typename T> Arg& type();

    // Getters
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] const std::string& short_name() const;
    [[nodiscard]] const std::string& long_name() const;
    [[nodiscard]] const std::string& help() const;
    [[nodiscard]] bool is_required() const;
    [[nodiscard]] bool takes_value() const;
    [[nodiscard]] bool has_default() const;
    [[nodiscard]] const std::string& default_value() const;
    [[nodiscard]] std::any convert(const std::string& s) const;

  private:
    std::string name_;
    std::string short_;
    std::string long_;
    std::string help_;
    bool required_ = false;
    bool takes_value_ = true;
    std::string default_;
    std::function<std::any(const std::string&)> converter_;
};

// Template implementation must stay in header
template <typename T> Arg& Arg::type() {
    converter_ = [](const std::string& s) -> std::any {
        if constexpr (std::is_same_v<T, bool>) {
            if (s == "true" || s == "1") {
                return true;
            }
            if (s == "false" || s == "0") {
                return false;
            }
            throw std::invalid_argument("Invalid boolean value");
        } else if constexpr (std::is_integral_v<T>) {
            return static_cast<T>(std::stoll(s));
        } else if constexpr (std::is_floating_point_v<T>) {
            return static_cast<T>(std::stod(s));
        } else if constexpr (std::is_same_v<T, std::string>) {
            return s;
        } else {
            static_assert(!std::is_same_v<T, T>, "Unsupported type");
        }
    };
    return *this;
}
