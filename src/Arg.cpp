#include <cstdlib>
#include <optional>
#include <pthread.h>
#include <string>
#include <utility>

#include "../include/Arg.hpp"
#include "../src/utils.cpp"

Arg::Arg(std::string name) : name_(std::move(name)), long_(name_), required_(false), takes_value_(true), has_env_(false), try_env_(false), value_(std::nullopt)  {}

// Setters
Arg& Arg::short_name(const std::string& s) {
    short_ = s;
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
bool Arg::requires_value() const { return takes_value_; }
Arg& Arg::from_env(const char* env_var_name) {
    this->has_env_ = true;
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
Arg& Arg::try_env() {
    this -> try_env_ = true;
    return *this;
};

// Getters
const std::string& Arg::name() const { return name_; }
const std::string& Arg::short_name() const { return short_; }
const std::string& Arg::long_name() const { return long_; }
const std::string& Arg::help() const { return help_; }
bool Arg::is_required() const { return required_; }
bool Arg::takes_value() const { return takes_value_; }
bool Arg::has_default() const { return !default_.empty(); }
bool Arg::has_env() const { return has_env_; }
const std::string& Arg::default_value() const { return default_; }

std::ostream& operator<<(std::ostream& os, const Arg& arg) {
    os << "Arg {\n"
       << "  name: \"" << arg.name_ << "\",\n"
       << "  short: \"" << arg.short_ << "\",\n"
       << "  long: \"" << arg.long_ << "\",\n"
       << "  help: \"" << arg.help_ << "\",\n"
       << "  required: " << std::boolalpha << arg.required_ << ",\n"
       << "  takes_value: " << std::boolalpha << arg.takes_value_ << ",\n"
       << "  default: \"" << arg.default_ << "\",\n"
       << "  value: ";
    if (arg.value_)
        os << "\"" << arg.value_.value() << "\"";
    else
        os << "nullopt";
    os << "\n}";
    return os;
}

void Arg::set_try_env_name(const std::string& s){
    this->try_env_name_ = s;
}
