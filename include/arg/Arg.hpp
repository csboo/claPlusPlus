#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include "Macros.hpp"
#include "BaseArg.hpp"
#include "utils.hpp"

// === Type state flags ===
struct NotSet {};
struct Set {};

// sth
template<typename...>
struct CountMismatches;

template<typename T1, typename T2, typename... Rest>
struct CountMismatches<T1, T2, Rest...> {
    static constexpr int value = (!std::is_same_v<T1, T2>) + CountMismatches<Rest...>::value;
};

template<>
struct CountMismatches<> {
    static constexpr int value = 0;
};

// === Builder class with full type-state enforcement ===
template <typename ShortName = NotSet, typename LongName = NotSet, typename Help = NotSet, typename IsRequired = NotSet,
          typename IsFlag = NotSet, typename AcceptsMany = NotSet, typename DefaultValue = NotSet,
          typename FromEnv = NotSet, typename AutoEnv = NotSet>
class Arg final : public BaseArg {
   public:
    Arg(std::string name) {
        name_ = std::move(name);
        long_name_ = name_;
    }

    ~Arg() override = default;

    static inline std::unique_ptr<BaseArg> builder(const std::string& name) {
      return std::make_unique<Arg<>>(name);
    }
    
    template<typename SN, typename LN, typename H, typename IR, typename IF,
             typename AM, typename DV, typename FE, typename AE>
    friend class Arg;

    template<typename OSN, typename OLN, typename OH, typename OIR, typename OIF,
             typename OAM, typename ODV, typename OFE, typename OAE>
    Arg(const Arg<OSN, OLN, OH, OIR, OIF, OAM, ODV, OFE, OAE>& other) requires (CountMismatches<
                     ShortName, OSN,
                     LongName, OLN,
                     Help, OH,
                     IsRequired, OIR,
                     IsFlag, OIF,
                     AcceptsMany, OAM,
                     DefaultValue, ODV,
                     FromEnv, OFE,
                     AutoEnv, OAE
                 >::value == 1) {
        name_ = other.name_;
        long_name_ = other.long_name_;
        help_ = other.help_;
        is_required_ = other.is_required_;
        is_flag_ = other.is_flag_;
        accepts_many_ = other.accepts_many_;
        value_ = other.value_;
        auto_env_ = other.auto_env_;
    }

    ARG_USER_CUSTOM_FUNCTION_SAFE(short_name, std::string, ShortName, "Error: don't call short_name() more than once!",
                                  Set, LongName, Help, IsRequired, IsFlag, AcceptsMany, DefaultValue, FromEnv,
                                  AutoEnv)
    ARG_USER_CUSTOM_FUNCTION_SAFE(long_name, std::string, LongName, "Error: don't call long_name() more than once!",
                                  ShortName, Set, Help, IsRequired, IsFlag, AcceptsMany, DefaultValue, FromEnv,
                                  AutoEnv)
    ARG_USER_CUSTOM_FUNCTION_SAFE(help, std::string, Help, "Error: don't call help() more than once!", ShortName,
                                  LongName, Set, IsRequired, IsFlag, AcceptsMany, DefaultValue, FromEnv, AutoEnv)
    ARG_USER_BOOL_FUNCTION_SAFE(is_required, IsRequired, true, "Error: don't call is_required() more than once!",
                                ShortName, LongName, Help, Set, IsFlag, AcceptsMany, DefaultValue, FromEnv,
                                AutoEnv)
    ARG_USER_BOOL_FUNCTION_SAFE(is_flag, IsFlag, true, "Error: don't call is_flag() more than once!", ShortName,
                                LongName, Help, IsRequired, Set, AcceptsMany, DefaultValue, FromEnv, AutoEnv)
    ARG_USER_BOOL_FUNCTION_SAFE(accepts_many, AcceptsMany, true, "Error: don't call accepts_many() more than once!",
                                ShortName, LongName, Help, IsRequired, IsFlag, Set, DefaultValue, FromEnv,
                                AutoEnv)
    ARG_USER_CUSTOM_FUNCTION_SAFE(default_value, std::string, DefaultValue,
                                  "Error: don't call default_value() more than once!", ShortName, LongName, Help,
                                  IsRequired, IsFlag, AcceptsMany, Set, FromEnv, AutoEnv)
    ARG_USER_BOOL_FUNCTION_SAFE(auto_env, AutoEnv, true, "Error: don't call auto_env() more than once!", ShortName,
                                LongName, Help, IsRequired, IsFlag, AcceptsMany, DefaultValue, FromEnv, Set)

    // ARG_USER_BOOL_FUNCTION_SAFE(from_env, FromEnv, false, "Error: don't call from_env() more than once!")

   private:
    friend class ClapParser;

    // std::string name_;
    // std::string short_name_;
    // std::string long_name_;
    // std::string help_;
    // bool is_required_;
    // bool is_flag_;
    // bool accepts_many_;
    // std::string env_name_;
    // bool auto_env_;
    // std::string default_value_;
    // std::optional<std::string> value_;

    void print_arg(std::ostream& os, int indent = 0) const override {
        print_indent(os, indent);
        os << "Arg {\n";

        print_indent(os, indent + 1);
        os << "name: \"" << name_ << "\",\n";
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

    friend std::ostream& operator<<(std::ostream& os, const BaseArg& arg);

    // ----| Getters & Setters |----
    DEFINE_GETTER_SETTER_OVERRIDE(name, std::string)
    DEFINE_GETTER_SETTER_OVERRIDE(short_name, std::string)
    DEFINE_GETTER_SETTER_OVERRIDE(long_name, std::string)
    DEFINE_GETTER_SETTER_OVERRIDE(help, std::string)
    DEFINE_GETTER_SETTER_OVERRIDE(is_required, bool)
    DEFINE_GETTER_SETTER_OVERRIDE(is_flag, bool)
    DEFINE_GETTER_SETTER_OVERRIDE(accepts_many, bool)
    DEFINE_GETTER_SETTER_OVERRIDE(env_name, std::string)
    DEFINE_GETTER_SETTER_OVERRIDE(auto_env, bool)
    DEFINE_GETTER_SETTER_OVERRIDE(default_value, std::string)
    DEFINE_GETTER_SETTER_OVERRIDE(value, std::optional<std::string>)

    // ----| Checkers |----
    // has_env_
    [[nodiscard]] bool has_env() const override { return !this->env_name_.empty(); }

    // has_default_
    [[nodiscard]] bool has_default() const override { return !this->default_value_.empty(); }

    // has_value_
    [[nodiscard]] bool has_value() const override { return this->value_.has_value(); }
};
