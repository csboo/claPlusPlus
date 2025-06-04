#pragma once

#include <charconv>
#include <cstdint>
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
DEFINE_PARSABLE_NUM_TYPE(int8_t)
DEFINE_PARSABLE_NUM_TYPE(uint8_t)
DEFINE_PARSABLE_NUM_TYPE(int16_t)
DEFINE_PARSABLE_NUM_TYPE(uint16_t)
DEFINE_PARSABLE_NUM_TYPE(int32_t)
DEFINE_PARSABLE_NUM_TYPE(uint32_t)
DEFINE_PARSABLE_NUM_TYPE(int64_t)
DEFINE_PARSABLE_NUM_TYPE(uint64_t)

// Floating-point types
template <>
struct Parse<float> {
    static std ::optional<float> parse(std ::string_view s) {
        char* end = nullptr;
        float value = std::strtof(s.data(), &end);
        if (end == s.data() + s.size()) return value;
        return std::nullopt;
    }
};
template <>
struct Parse<double> {
    static std ::optional<double> parse(std ::string_view s) {
        char* end = nullptr;
        double value = std::strtod(s.data(), &end);
        if (end == s.data() + s.size()) return value;
        return std::nullopt;
    }
};
template <>
struct Parse<long double> {
    static std ::optional<long double> parse(std ::string_view s) {
        char* end = nullptr;
        long double value = std::strtold(s.data(), &end);
        if (end == s.data() + s.size()) return value;
        return std::nullopt;
    }
};

template <>
struct Parse<std::string> {
    static std::optional<std::string> parse(std::string_view s) { return std::string(s.data(), s.data() + s.size()); }
};

template <>
struct Parse<bool> {
    static std::optional<bool> parse(std::string_view s) {
        auto value = Parse<int>::parse(s);
        if (!value.has_value()) {
            return std::nullopt;
        }
        if (value.value() == 0) {
            return std::optional{ false };
        }
        return std::optional{ true };
    }
};
