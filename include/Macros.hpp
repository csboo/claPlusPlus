#pragma once

#define DEFINE_PARSABLE_INTEGER_TYPE(TYPE)                                          \
    template <>                                                                     \
    struct Parse<TYPE> {                                                            \
        static std::optional<TYPE> parse(std::string_view s) {                      \
            TYPE value = 0;                                                             \
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value); \
            return ec == std::errc() ? std::nullopt : std::optional{value};         \
        }                                                                           \
    };

#define DEFINE_PARSABLE_FLOAT_TYPE(TYPE)                                                                           \
    template <>                                                                                                    \
    struct Parse<TYPE> {                                                                                           \
        static std::optional<TYPE> parse(std::string_view s) {                                                     \
            TYPE value = 0;                                                                                            \
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, std::chars_format::scientific); \
            return ec == std::errc() ? std::nullopt : std::optional{value};                                        \
        }                                                                                                          \
    };

#define DEFINE_GETTER_SETTER(NAME, TYPE)                                           \
    [[nodiscard]] inline const TYPE& get__##NAME() const { return this->NAME##_; } \
    inline void set__##NAME(const TYPE& NAME) { this->NAME##_ = NAME; }
