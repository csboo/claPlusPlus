#include <optional>
#include <stdexcept>
#include <string>

template <typename T, typename E>
T ok_or(std::optional<T> opt, E&& err) {
    if (!opt) std::forward<E>(err)();
    return *opt;
}
template <typename T>
T ok_or_throw_str(std::optional<T> opt, const std::string& err) {
    if (!opt) throw std::runtime_error(err);
    return *opt;
}
