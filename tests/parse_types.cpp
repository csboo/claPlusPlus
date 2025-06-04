#include <cassert>

#include "Parser.hpp"

int main(int argc, char* argv[]) {
  ClapParser p;

  auto i8 = Arg().long_name("i8").default_value("127");
  auto i16 = Arg().long_name("i16").default_value("32767");
  auto i32 = Arg().long_name("i32").default_value("2147483647");
  auto i64 = Arg().long_name("i64").default_value("9223372036854775807");

  auto u8 = Arg().long_name("u8").default_value("255");
  auto u16 = Arg().long_name("u16").default_value("65535");
  auto u32 = Arg().long_name("u32").default_value("4294967295");
  auto u64 = Arg().long_name("u64").default_value("18446744073709551615");
  
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

  assert(p.get_one_as<int8_t>("i8").value() == 127);
  assert(p.get_one_as<int16_t>("i16").value() == 32767);
  assert(p.get_one_as<int32_t>("i32").value() == 2147483647);
  assert(p.get_one_as<int64_t>("i64").value() == 9223372036854775807LL);

  assert(p.get_one_as<uint8_t>("u8").value() == 255);
  assert(p.get_one_as<uint16_t>("u16").value() == 65535);
  assert(p.get_one_as<uint32_t>("u32").value() == 4294967295U);
  assert(p.get_one_as<uint64_t>("u64").value() == 18446744073709551615ULL);
}
