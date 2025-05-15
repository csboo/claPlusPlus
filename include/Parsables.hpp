#pragma once

#include "Macros.hpp"

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>

template <typename T> struct Parse {
    static_assert(sizeof(T) == 0, "No Parse<T> specialization defined for this type");
};

template <typename T>
concept Parseable = requires(std::string_view s) {
    { Parse<T>::parse(s) } -> std::convertible_to<std::optional<T>>;
};

// Integer types
DEFINE_PARSABLE_BASIC_TYPE(int8_t)
DEFINE_PARSABLE_BASIC_TYPE(uint8_t)
DEFINE_PARSABLE_BASIC_TYPE(int16_t)
DEFINE_PARSABLE_BASIC_TYPE(uint16_t)
DEFINE_PARSABLE_BASIC_TYPE(int32_t)
DEFINE_PARSABLE_BASIC_TYPE(uint32_t)
DEFINE_PARSABLE_BASIC_TYPE(int64_t)
DEFINE_PARSABLE_BASIC_TYPE(uint64_t)

// Floating-point types
DEFINE_PARSABLE_FLOAT_TYPE(float, std::strtof)
DEFINE_PARSABLE_FLOAT_TYPE(double, std::strtod)
DEFINE_PARSABLE_FLOAT_TYPE(long double, std::strtold)

template <> struct Parse<std::string> {
    static std::optional<std::string> parse(std::string_view s) { return std::string(s.data()); }
};

template <> struct Parse<bool> {
    static std::optional<bool> parse(std::string_view s) {
        auto as_int = Parse<int>::parse(s).value();
        return as_int;
    }
};
