#include <any>
#include <functional>
#include <stdexcept>
#include <string>

class Arg {
  public:
    Arg(const std::string& name);

    Arg& short_name(const std::string& s);
    Arg& long_name(const std::string& l);
    Arg& help(const std::string& help);
    Arg& required(bool is_required);
    Arg& takes_value(bool takes);
    Arg& default_value(const std::string& default_val);

    template <typename T> Arg& type();

    const std::string& name() const;
    const std::string& short_name() const;
    const std::string& long_name() const;
    const std::string& help() const;
    bool is_required() const;
    bool takes_value() const;
    bool has_default() const;
    const std::string& default_value() const;
    std::any convert(const std::string& s) const;

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

// Constructor
Arg::Arg(const std::string& name) : name_(name) {
    // Default converter to string
    converter_ = [](const std::string& s) -> std::any { return s; };
}

// Setters
Arg& Arg::short_name(const std::string& s) {
    short_ = s;
    return *this;
}
Arg& Arg::long_name(const std::string& l) {
    long_ = l;
    return *this;
}
Arg& Arg::help(const std::string& h) {
    help_ = h;
    return *this;
}
Arg& Arg::required(bool is_required) {
    required_ = is_required;
    return *this;
}
Arg& Arg::takes_value(bool takes) {
    takes_value_ = takes;
    return *this;
}
Arg& Arg::default_value(const std::string& default_val) {
    default_ = default_val;
    return *this;
}

// Type conversion
template <typename T> Arg& Arg::type() {
    converter_ = [](const std::string& s) -> std::any {
        if constexpr (std::is_same_v<T, bool>) {
            if (s == "true" || s == "1")
                return true;
            if (s == "false" || s == "0")
                return false;
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

// Getters
const std::string& Arg::name() const { return name_; }
const std::string& Arg::short_name() const { return short_; }
const std::string& Arg::long_name() const { return long_; }
const std::string& Arg::help() const { return help_; }
bool Arg::is_required() const { return required_; }
bool Arg::takes_value() const { return takes_value_; }
bool Arg::has_default() const { return !default_.empty(); }
const std::string& Arg::default_value() const { return default_; }
std::any Arg::convert(const std::string& s) const { return converter_(s); }
