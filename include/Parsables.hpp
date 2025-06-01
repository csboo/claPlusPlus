#pragma once

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>

#include "Macros.hpp"

template <typename T>
struct Parse {
    static_assert(sizeof(T) == 0, "No Parse<T> specialization defined for this type");
};

template <typename T>
concept Parseable = requires(std::string_view s) {
    { Parse<T>::parse(s) } -> std::convertible_to<std::optional<T>>;
};

// Integer types
DEFINE_PARSABLE_INTEGER_TYPE(int8_t)
DEFINE_PARSABLE_INTEGER_TYPE(uint8_t)
DEFINE_PARSABLE_INTEGER_TYPE(int16_t)
DEFINE_PARSABLE_INTEGER_TYPE(uint16_t)
DEFINE_PARSABLE_INTEGER_TYPE(int32_t)
DEFINE_PARSABLE_INTEGER_TYPE(uint32_t)
DEFINE_PARSABLE_INTEGER_TYPE(int64_t)
DEFINE_PARSABLE_INTEGER_TYPE(uint64_t)

// Floating-point types
DEFINE_PARSABLE_FLOAT_TYPE(float)
DEFINE_PARSABLE_FLOAT_TYPE(double)
DEFINE_PARSABLE_FLOAT_TYPE(long double)

template <>
struct Parse<std::string> {
    static std::optional<std::string> parse(std::string_view s) { return std::string(s.data(), s.data() + s.size()); }
};

template <>
struct Parse<bool> {
    static std::optional<bool> parse(std::string_view s) {
        auto as_int = Parse<int>::parse(s).value();
        return as_int;
    }
};
