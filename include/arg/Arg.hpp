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

// === Builder class with full type-state enforcement ===
template <typename ShortName = NotSet, typename LongName = NotSet, typename Help = NotSet, typename IsRequired = NotSet,
          typename IsFlag = NotSet, typename AcceptsMany = NotSet, typename DefaultValue = NotSet,
          typename FromEnv = NotSet, typename AutoEnv = NotSet>
class Arg final : public BaseArg {
   public:
    Arg() = default;
    ~Arg() override = default;

    template<typename SN, typename LN, typename H, typename IR, typename IF,
             typename AM, typename DV, typename FE, typename AE>
    friend class Arg;

    template<typename... States>
    Arg(const Arg<States...>& other) {
        // name_ = std::move(other.name_);
        short_name_ = std::move(other.short_name_);
        long_name_ = std::move(other.long_name_);
        help_ = std::move(other.help_);
        is_required_ = std::move(other.is_required_);
        is_flag_ = std::move(other.is_flag_);
        default_value_ = std::move(other.default_value_);
        accepts_many_ = std::move(other.accepts_many_);
        value_ = std::move(other.value_);
        auto_env_ = std::move(other.auto_env_);
        env_name_ = std::move(other.env_name_);
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

    // this is expanded for its different functionality
    [[nodiscard]] inline auto from_env(std::string env_var_name) {
        static_assert(std ::is_same_v<FromEnv, NotSet>, "Error: don't call auto_env() more than once!");
        this->env_name_ = std::move(env_var_name);
        Arg<ShortName, LongName, Help, IsRequired, IsFlag, AcceptsMany, DefaultValue, Set, AutoEnv> next =
            std ::move(*this);
        return next;
    }

   private:
    friend class ClapParser;

    friend std::ostream& operator<<(std::ostream& os, const BaseArg& arg);

    // ----| Getters & Setters |----
    // DEFINE_GETTER_SETTER_OVERRIDE(name, std::string)
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
