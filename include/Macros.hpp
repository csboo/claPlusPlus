#pragma once

#define DEFINE_PARSABLE_NUM_TYPE(TYPE)                                          \
    template <>                                                                     \
    struct Parse<TYPE> {                                                            \
        static std::optional<TYPE> parse(std::string_view s) {                      \
            TYPE value = 0;                                                             \
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value); \
            return ec == std::errc() ? std::optional{value} : std::nullopt;         \
        }                                                                           \
    };

// #define DEFINE_PARSABLE_FLOAT_TYPE(TYPE)                                                                           \
//     template <>                                                                                                    \
//     struct Parse<TYPE> {                                                                                           \
//         static std::optional<TYPE> parse(std::string_view s) {                                                     \
//             TYPE value = 0;                                                                                            \
//             auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, std::chars_format::scientific); \
//             return ec == std::errc() ? std::optional{value} : std::nullopt;                                        \
//         }                                                                                                          \
//     };

#define DEFINE_GETTER_SETTER_OVERRIDE(NAME, TYPE)                                           \
    [[nodiscard]] inline const TYPE& get__##NAME() const override { return this->NAME##_; } \
    inline void set__##NAME(const TYPE& NAME) override { this->NAME##_ = NAME; }

#define DEFINE_GETTER_SETTER_VIRTUAL(NAME, TYPE)               \
    [[nodiscard]] virtual const TYPE& get__##NAME() const = 0; \
    virtual void set__##NAME(const TYPE& NAME) = 0;

#define ARG_USER_BOOL_FUNCTION_SAFE(NAME, TARGET_STATE, VALUE, ERROR_MSG, ...) \
    [[nodiscard]] inline auto NAME() {                             \
        static_assert(std ::is_same_v<LongName, Set>, "Error: I need a name! [.long_name(), .short_name()]"); \
        static_assert(std::is_same_v<TARGET_STATE, NotSet>, ERROR_MSG);        \
        this->NAME##_ = VALUE;                                                 \
        Arg<__VA_ARGS__> next = std::move(*this);                                         \
        return next;                                                           \
    }

#define ARG_USER_CUSTOM_FUNCTION_SAFE(NAME, TYPE, TARGET_STATE, ERROR_MSG, ...) \
    [[nodiscard]] inline auto NAME(TYPE NAME) {               \
        static_assert(std::is_same_v<TARGET_STATE, NotSet>, ERROR_MSG);         \
        this->NAME##_ = std::move(NAME);                                        \
        Arg<__VA_ARGS__> next = *this;                                          \
        return next;                                                            \
    }

#define ARG_USER_CUSTOM_FUNCTION_SAFE_ISNAMED(NAME, TYPE, TARGET_STATE, ERROR_MSG, ...) \
    [[nodiscard]] inline auto NAME(TYPE NAME) {               \
        static_assert(std ::is_same_v<LongName, Set>, "Error: I need a name! [.long_name(), .short_name()]"); \
        static_assert(std::is_same_v<TARGET_STATE, NotSet>, ERROR_MSG);         \
        this->NAME##_ = std::move(NAME);                                        \
        Arg<__VA_ARGS__> next = *this;                                          \
        return next;                                                            \
    }
