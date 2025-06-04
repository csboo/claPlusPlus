#pragma once

#define DEFINE_PARSABLE_INTEGER_TYPE(TYPE)                                          \
    template <>                                                                     \
    struct Parse<TYPE> {                                                            \
        static std::optional<TYPE> parse(std::string_view s) {                      \
            TYPE value = 0;                                                             \
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value); \
            return ec == std::errc() ? std::optional{value} : std::nullopt;         \
        }                                                                           \
    };

#define DEFINE_PARSABLE_FLOAT_TYPE(TYPE)                                                                           \
    template <>                                                                                                    \
    struct Parse<TYPE> {                                                                                           \
        static std::optional<TYPE> parse(std::string_view s) {                                                     \
            TYPE value = 0;                                                                                            \
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, std::chars_format::scientific); \
            return ec == std::errc() ? std::optional{value} : std::nullopt;                                        \
        }                                                                                                          \
    };

#define DEFINE_GETTER_SETTER_OVERRIDE(NAME, TYPE)                                           \
    [[nodiscard]] inline const TYPE& get__##NAME() const override { return this->NAME##_; } \
    inline void set__##NAME(const TYPE& NAME) override { this->NAME##_ = NAME; }

#define DEFINE_GETTER_SETTER_VIRTUAL(NAME, TYPE)               \
    [[nodiscard]] virtual const TYPE& get__##NAME() const = 0; \
    virtual void set__##NAME(const TYPE& NAME) = 0;

#define ARG_USER_BOOL_FUNCTION_SAFE(NAME, TARGET_STATE, VALUE, ERROR_MSG, ...) \
    [[nodiscard]] inline Arg<__VA_ARGS__> NAME() {                             \
        static_assert(std::is_same_v<TARGET_STATE, NotSet>, ERROR_MSG);        \
        this->NAME##_ = VALUE;                                                 \
        Arg<__VA_ARGS__> next = std::move(*this);                                         \
        return next;                                                           \
    }

#define ARG_USER_CUSTOM_FUNCTION_SAFE(NAME, TYPE, TARGET_STATE, ERROR_MSG, ...) \
    [[nodiscard]] inline Arg<__VA_ARGS__> NAME(const TYPE NAME) {               \
        static_assert(std::is_same_v<TARGET_STATE, NotSet>, ERROR_MSG);         \
        this->NAME##_ = std::move(NAME);                                        \
        Arg<__VA_ARGS__> next = *this;                                          \
        return next;                                                            \
    }

#define ARG_STATE_CONVERSION_CTOR(ALLOW_DIFF_IDX)                                                              \
    template <typename OSN, typename OLN, typename OH, typename OIR, typename OIF, typename OAM, typename ODV, \
              typename OFE, typename OAE,                                                                      \
              std::enable_if_t<(ALLOW_DIFF_IDX != 0 || std::is_same_v<ShortName, OSN>) &&                      \
                               (ALLOW_DIFF_IDX != 1 || std::is_same_v<LongName, OLN>) &&                       \
                               (ALLOW_DIFF_IDX != 2 || std::is_same_v<Help, OH>) &&                            \
                               (ALLOW_DIFF_IDX != 3 || std::is_same_v<IsRequired, OIR>) &&                     \
                               (ALLOW_DIFF_IDX != 4 || std::is_same_v<IsFlag, OIF>) &&                         \
                               (ALLOW_DIFF_IDX != 5 || std::is_same_v<AcceptsMany, OAM>) &&                    \
                               (ALLOW_DIFF_IDX != 6 || std::is_same_v<DefaultValue, ODV>) &&                   \
                               (ALLOW_DIFF_IDX != 7 || std::is_same_v<FromEnv, OFE>) &&                        \
                               (ALLOW_DIFF_IDX != 8 || std::is_same_v<AutoEnv, OAE>)>* = nullptr>              \
    Arg(const Arg<OSN, OLN, OH, OIR, OIF, OAM, ODV, OFE, OAE>& other) {                                        \
        this->name_ = other.name_;                                                                             \
        this->long_name_ = other.long_name_;                                                                   \
        this->help_ = other.help_;                                                                             \
        this->is_required_ = other.is_required_;                                                               \
        this->is_flag_ = other.is_flag_;                                                                       \
        this->accepts_many_ = other.accepts_many_;                                                             \
        this->value_ = other.value_;                                                                           \
        this->auto_env_ = other.auto_env_;                                                                     \
    }
