// tests/test_flag.cpp
#include <cassert>
#include <cstdlib>
#include <string>

#include "Arg.hpp"
#include "Parser.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
    ClapParser p;
    auto f = Arg("opt").is_flag();
    p.add_arg(f);
    p.parse(argc, argv);

    // EXPECTED should be "1" or "0" (present or absent)
    const auto e = std::getenv("EXPECTED");
    assert(e && "EXPECTED must be set");
    bool expected = std::string(e) == "1";

    auto actual = ok_or_throw_str(p.get_one_as<bool>("opt"), "test argument: 'opt' is missing");
    std::cerr << p << '\n';
    assert(actual == expected);
    return 0;
}
