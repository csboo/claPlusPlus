#include "Arg.hpp"
#include "utils.hpp"

#include <optional>
#include <pthread.h>
#include <string>

Arg::Arg(std::string name)
    : name_(std::move(name)), long_name_(this->name_), is_required_(false), is_flag_(false), accepts_many_(false),
      auto_env_(false), value_(std::nullopt) {}

// Setters
Arg& Arg::short_name(const std::string& short_name) {
    short_name_ = short_name;
    return *this;
}
Arg& Arg::help(const std::string& help) {
    help_ = help;
    return *this;
}
Arg& Arg::required(bool is_required) {
    is_required_ = is_required;
    return *this;
}
Arg& Arg::is_flag() {
    is_flag_ = true;
    default_value_ = "0";
    return *this;
}
Arg& Arg::accepts_many() {
    accepts_many_ = true;
    return *this;
}
Arg& Arg::default_value(const std::string& default_value) {
    default_value_ = default_value;
    return *this;
}
Arg& Arg::from_env(const char* env_var_name) {
    this->env_name_ = env_var_name;
    return *this;
};
Arg& Arg::auto_env() {
    this->auto_env_ = true;
    return *this;
};

void Arg::print_arg(std::ostream& os, const Arg& arg, int indent) {
    print_indent(os, indent);
    os << "Arg {\n";

    print_indent(os, indent + 1);
    os << "name: \"" << arg.name_ << "\",\n";
    print_indent(os, indent + 1);
    os << "short: \"" << arg.short_name_ << "\",\n";
    print_indent(os, indent + 1);
    os << "long: \"" << arg.long_name_ << "\",\n";
    print_indent(os, indent + 1);
    os << "help: \"" << arg.help_ << "\",\n";
    print_indent(os, indent + 1);
    os << "required: " << std::boolalpha << arg.is_required_ << ",\n";
    print_indent(os, indent + 1);
    os << "is_flag: " << std::boolalpha << arg.is_flag_ << ",\n";
    print_indent(os, indent + 1);
    os << "accepts_many: " << std::boolalpha << arg.accepts_many_ << ",\n";
    print_indent(os, indent + 1);
    os << "default: \"" << arg.default_value_ << "\",\n";
    print_indent(os, indent + 1);
    os << "value: ";
    if (arg.value_) {
        os << "\"" << arg.value_.value() << "\"";
    } else {
        os << "std::nullopt";
    }
    os << '\n';

    print_indent(os, indent);
    os << "}";
}

std::ostream& operator<<(std::ostream& os, const Arg& arg) {
    Arg::print_arg(os, arg, 0);
    return os;
}
