#include <catch.hpp>

#include <out/csv.h>

struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{};
};

TEST_CASE("write CSV output", "[out]")
{
#if __cpp_lib_reflection >= 201902
   std::vector<user> users{
      { "John Doe", "secret", 42, 0 },
      { "Max Mustermann", "****", 1, 45'678 }
   };

   const auto result = out::csv(users);
   const auto expected = R"(name, password, karma, cash
John Doe, secret, 42, 0
Max Mustermann, ****, 1, 45678
)";

   REQUIRE(result == expected);
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}
