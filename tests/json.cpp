#include <catch.hpp>

#include <out/json.h>

struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{};
};

TEST_CASE("write JSON output", "[out][json]")
{
#if __cpp_lib_reflection >= 201902
   //! \todo
   REQUIRE(false);
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}
