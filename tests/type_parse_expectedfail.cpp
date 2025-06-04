#include <iostream>
#include <optional>

#include "Parser.hpp"

int main(int argc, char* argv[]) {
    ClapParser p;

    auto i8 = Arg().long_name("i8").default_value("128");
    auto i16 = Arg().long_name("i16").default_value("32768");
    auto i32 = Arg().long_name("i32").default_value("2147483648");
    auto i64 = Arg().long_name("i64").default_value("9223372036854775808");

    auto u8 = Arg().long_name("u8").default_value("256");
    auto u16 = Arg().long_name("u16").default_value("65536");
    auto u32 = Arg().long_name("u32").default_value("4294967296");
    auto u64 = Arg().long_name("u64").default_value("18446744073709551616");

    // auto f32 = Arg().long_name("f32").default_value("");
    // auto f64 = Arg().long_name("f64").default_value("");
    // auto d64 = Arg().long_name("d64").default_value("");

    p.add_arg(u64);
    p.add_arg(u32);
    p.add_arg(u16);
    p.add_arg(u8);
    p.add_arg(i64);
    p.add_arg(i32);
    p.add_arg(i16);
    p.add_arg(i8);

    p.parse(argc, argv);
    std::cerr << p << "\n";

    // If no exception or failure occurs, check each manually
    bool anyParsed = false;
    auto try_parse = [&](const std::string& name, auto type_tag) {
        try {
            auto overflown = p.get_one_as<decltype(type_tag)>(name).value();

            std::cerr << "ERROR: " << name << " should have failed but parsed successfully with: \'" << overflown << "\'\n";
            anyParsed = true;
        } catch (std::bad_optional_access& bc) {
            std::cerr << name << " failed as expected.\n";
        }
    };

    try_parse("i8", int8_t{});
    try_parse("i16", int16_t{});
    try_parse("i32", int32_t{});
    try_parse("i64", int64_t{});
    try_parse("u8", uint8_t{});
    try_parse("u16", uint16_t{});
    try_parse("u32", uint32_t{});
    try_parse("u64", uint64_t{});

    if (!anyParsed) {
        std::cerr << "All overflow values failed as expected.\n";
    } else {
        std::cerr << "Some overflow values were accepted when they shouldn't be.\n";
        return 1;
    }
}
