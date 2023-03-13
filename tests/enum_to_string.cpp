#include <catch.hpp>

#include <out/enum_to_string.h>

enum class DaysOfTheWeek { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

TEST_CASE("convert enum to string", "[out]")
{
// #if __cpp_lib_reflection >= 201902
#if __has_include(<experimental/reflect>)
   std::string s = to_string(DaysOfTheWeek::Friday);
   REQUIRE(s == "Friday");
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}

TEST_CASE("convert string to enum", "[out]")
{
#if __has_include(<experimental/reflect>)
// #if __cpp_lib_reflection >= 201902
   const auto v = to_enum<DaysOfTheWeek>("Sunday");
   REQUIRE(v == DaysOfTheWeek::Sunday);
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}

struct case_insensitive_compare
{
   bool operator()(std::string_view lhs, std::string_view rhs)
   {
      return std::equal(begin(lhs), end(lhs), begin(rhs), end(rhs),
                        [](char a, char b) { return tolower(a) == tolower(b); });
   }
};

TEST_CASE("convert string (case insensitive compare) to enum", "[out]")
{
#if __has_include(<experimental/reflect>)
// #if __cpp_lib_reflection >= 201902
   const auto v = to_enum<DaysOfTheWeek, case_insensitive_compare>("sunday");
   REQUIRE(v == DaysOfTheWeek::Sunday);
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}