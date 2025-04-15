#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <sstream>

template <typename T, typename E>
inline T ok_or(std::optional<T> opt, E&& err) {
    if (!opt) std::forward<E>(err)();
    return *opt;
}

template <typename T>
inline T ok_or_throw_str(std::optional<T> opt, const std::string& err) {
    if (!opt) throw std::runtime_error(err);
    return *opt;
}

template <typename T>
inline T ptr_ok_or_throw_str(T pointer, const std::string& err) {
    if (!pointer) throw std::runtime_error(err);
    return pointer;
}

template <typename P, typename E>
inline E ptr_unwrap_or(P pointer, const E other) { 
    if (!pointer) return other;
}
 
// variadic template function to concatenate any number of arguments
template <typename... Args> inline std::string concat(Args&&... args) {
    std::ostringstream oss;
    (void)std::initializer_list<int>{
        (oss << std::forward<Args>(args), 0)...}; // using initializer_list for fold-like behavior
    return oss.str();
}

inline const std::string PROGRAM_NAME() {
   const std::string& raw_program_name = program_invocation_name;
    return raw_program_name.substr(raw_program_name.rfind('/') + 1);
}
