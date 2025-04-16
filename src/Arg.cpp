#include "Arg.hpp"

#include <cstdlib>
#include <optional>
#include <pthread.h>
#include <string>

Arg::Arg(const std::string& name) :
    name_(name),
    short_name_(""),
    long_name_(this->name_),
    help_(""),
    is_required_(false),
    is_flag_(false),
    env_name_(""),
    auto_env_(false),
    default_value_(""),
    value_(std::nullopt)
{}

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
Arg& Arg::is_flag(bool is_flag)  {
    is_flag_ = is_flag;
    return *this;
}
Arg& Arg::default_value(const std::string& default_value) {
    default_value_ = default_value;
    return *this;
}
Arg& Arg::from_env(const char* env_var_name) {
    this->env_name_ = env_var_name;
    // std::string value_from_env = ptr_unwrap_or(std::getenv(env_var_name), concat("value \'", env_var_name, "\' not present in env for: ", this->name_));
    // std::optional<std::string> value_from_env = ptr_unwrap_or(std::getenv(env_var_name), std::nullopt);
    // this->value_ = ptr_unwrap_or(std::getenv(env_var_name), std::nullopt);
    auto ptr = std::getenv(env_var_name);
    if (!ptr) {
        this->value_ = std::nullopt;
    } else {
        this->value_ = ptr;
    }
    return *this;    
};
Arg& Arg::auto_env() {
    this -> auto_env_ = true;
    return *this;
};

std::ostream& operator<<(std::ostream& os, const Arg& arg) {
    os << "Arg {\n"
       << "  name: \"" << arg.name_ << "\",\n"
       << "  short: \"" << arg.short_name_ << "\",\n"
       << "  long: \"" << arg.long_name_ << "\",\n"
       << "  help: \"" << arg.help_ << "\",\n"
       << "  required: " << std::boolalpha << arg.is_required_ << ",\n"
       << "  is_flag: " << std::boolalpha << arg.is_flag_ << ",\n"
       << "  default: \"" << arg.default_value_ << "\",\n"
       << "  value: ";
    if (arg.value_)
        os << "\"" << arg.value_.value() << "\"";
    else
        os << "std::nullopt";
    os << "\n}";
    return os;
}
