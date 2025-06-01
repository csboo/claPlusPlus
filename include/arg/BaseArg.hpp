// BaseArg.hpp
#pragma once

#include "Macros.hpp"
#include <optional>
#include <string>

class BaseArg {
  public:
    BaseArg() = default;
    virtual ~BaseArg();

    virtual void print_arg(std::ostream& os, int indent = 0) const = 0;

    DEFINE_GETTER_SETTER_VIRTUAL(name, std::string)
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
    std::string name_;
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
