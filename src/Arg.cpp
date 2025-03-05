#include <utility>

#include "include/Arg.hpp"

Arg::Arg(std::string name) : name_(std::move(name)) {
    // Default converter to string
    converter_ = [](const std::string& s) -> std::any { return s; };
}

// Setters
Arg& Arg::short_name(const std::string& s) { short_ = s; return *this; }
Arg& Arg::long_name(const std::string& l) { long_ = l; return *this; }
Arg& Arg::help(const std::string& h) { help_ = h; return *this; }
Arg& Arg::required(bool is_required) { required_ = is_required; return *this; }
Arg& Arg::takes_value(bool takes) { takes_value_ = takes; return *this; }
Arg& Arg::default_value(const std::string& default_val) { default_ = default_val; return *this; }

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
