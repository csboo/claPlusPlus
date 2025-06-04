// BaseArg.hpp
#pragma once

#include "Macros.hpp"
#include "utils.hpp"
#include <optional>
#include <string>

class BaseArg {
  public:
    BaseArg() = default;
    virtual ~BaseArg();

    void print_arg(std::ostream& os, int indent = 0) const {
        print_indent(os, indent);
        os << "Arg {\n";

        print_indent(os, indent + 1);
        os << "short: \"" << short_name_ << "\",\n";
        print_indent(os, indent + 1);
        os << "long: \"" << long_name_ << "\",\n";
        print_indent(os, indent + 1);
        os << "help: \"" << help_ << "\",\n";
        print_indent(os, indent + 1);
        os << "required: " << std::boolalpha << is_required_ << ",\n";
        print_indent(os, indent + 1);
        os << "is_flag: " << std::boolalpha << is_flag_ << ",\n";
        print_indent(os, indent + 1);
        os << "accepts_many: " << std::boolalpha << accepts_many_ << ",\n";
        print_indent(os, indent + 1);
        os << "default: \"" << default_value_ << "\",\n";
        print_indent(os, indent + 1);
        os << "auto_env: \"" << std::boolalpha << auto_env_ << "\",\n";
        print_indent(os, indent + 1);
        os << "env_name: \"" << env_name_ << "\",\n";
        print_indent(os, indent + 1);
        os << "value: ";
        if (value_) {
            os << "\"" << value_.value() << "\"";
        } else {
            os << "std::nullopt";
        }
        os << '\n';

        print_indent(os, indent);
        os << "}";
    }

    // DEFINE_GETTER_SETTER_VIRTUAL(name, std::string)
    DEFINE_GETTER_SETTER_VIRTUAL(short_name, std::string)
    DEFINE_GETTER_SETTER_VIRTUAL(long_name, std::string)
    DEFINE_GETTER_SETTER_VIRTUAL(help, std::string)
    DEFINE_GETTER_SETTER_VIRTUAL(is_required, bool)
    DEFINE_GETTER_SETTER_VIRTUAL(is_flag, bool)
    DEFINE_GETTER_SETTER_VIRTUAL(accepts_many, bool)
    DEFINE_GETTER_SETTER_VIRTUAL(env_name, std::string)
    DEFINE_GETTER_SETTER_VIRTUAL(auto_env, bool)
    DEFINE_GETTER_SETTER_VIRTUAL(default_value, std::string)
    DEFINE_GETTER_SETTER_VIRTUAL(value, std::optional<std::string>)
    
    // ----| Checkers |----
    // has_env_
    [[nodiscard]] virtual bool has_env() const = 0;

    // has_default_
    [[nodiscard]] virtual bool has_default() const = 0;

    // has_value_
    [[nodiscard]] virtual bool has_value() const = 0;

  protected:
    std::string short_name_;
    std::string long_name_;
    std::string help_;
    bool is_required_{};
    bool is_flag_{};
    bool accepts_many_{};
    std::string env_name_;
    bool auto_env_{};
    std::string default_value_;
    std::optional<std::string> value_ = std::nullopt;
};

inline std::ostream& operator<<(std::ostream& os, const BaseArg& arg) {
    arg.print_arg(os, 0);
    return os;
}
