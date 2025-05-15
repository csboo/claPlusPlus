// tests/test_priority.cpp
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "Arg.hpp"
#include "Parser.hpp"
#include "utils.hpp"

/* VALUE PRIORITY:
    1. cli arg: '--val'
    2. env var: 'VAL'
    3. auto env var: 'PRIORITY_MANUAL_VAL'
    4. default value: '1'
*/

int main(const int argc, char* argv[]) {
    ClapParser p;

    auto a = Arg("val").from_env("VAL").auto_env().default_value("1");
    p.add_arg(a);
    p.parse(argc, argv);

    // gets read from meson test env
    auto *const e = std::getenv("EXPECTED");
    assert(e && "EXPECTED must be set");
    int expected = std::stoi(e);

    auto actual = ok_or_throw_str(p.get_one_as<int>("val"), "test argument: 'val' is missing");
    std::cerr << p << '\n';
    assert(actual == expected);
    return 0;
}
