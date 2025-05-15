#pragma once

#define DEFINE_PARSABLE_BASIC_TYPE(TYPE)                                            \
    template <>                                                                     \
    struct Parse<TYPE> {                                                            \
        static std::optional<TYPE> parse(std::string_view s) {                      \
            TYPE value;                                                             \
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value); \
            if (ec == std::errc()) return value;                                    \
            return std::nullopt;                                                    \
        }                                                                           \
    };

#define DEFINE_PARSABLE_FLOAT_TYPE(TYPE, CONVERT_FN)           \
    template <>                                                \
    struct Parse<TYPE> {                                       \
        static std::optional<TYPE> parse(std::string_view s) { \
            char* end = nullptr;                               \
            TYPE value = CONVERT_FN(s.data(), &end);           \
            if (end == s.data() + s.size()) {                  \
                return value;                                  \
            }                                                  \
            return std::nullopt;                               \
        }                                                      \
    };

#define DEFINE_GETTER_SETTER(NAME, TYPE)                                           \
    [[nodiscard]] inline const TYPE& get__##NAME() const { return this->NAME##_; } \
    inline void set__##NAME(const TYPE& NAME) { this->NAME##_ = NAME; }
