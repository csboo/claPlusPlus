// tests/test_combo.cpp
#include <cassert>
#include <cstdlib>
#include <string>

#include "Arg.hpp"
#include "Parser.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
    ClapParser p;
    // a value arg with full priority stack:
    auto val = Arg("val").from_env("VAL").auto_env().default_value("10");
    auto boolean = Arg("flag").is_flag();
    p.add_arg(val);
    p.add_arg(boolean);
    p.parse(argc, argv);

    // EXPECTED_V   -> any int
    // EXPECTED_B   -> "1" or "0"
    auto *const ev = std::getenv("EXPECTED_V");
    auto *const eb = std::getenv("EXPECTED_B");
    assert(ev && eb && "EXPECTED_V/B must be set");

    int expected_val = std::stoi(ev);
    bool expected_boolean = std::stoi(eb) != 0;

    auto actual_val = ok_or_throw_str(p.get_one_as<int>("val"), "test argument: 'val' is missing");
    auto actual_boolean = ok_or_throw_str(p.get_one_as<bool>("flag"), "test argument: 'flag' is missing");

    std::cerr << p << '\n';
    assert(actual_val == expected_val);
    assert(actual_boolean == expected_boolean);
    return 0;
}
