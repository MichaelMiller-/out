#include <catch.hpp>

#include <out/sql/insert_into.h>

struct user
{
   std::string name{};
   std::string password{};
   double cash{};
   int karma{};
};

TEST_CASE("generate SQL-Insert statement", "[out][sql]")
{
#if __cpp_lib_reflection >= 201902
   const user u{ .name="John Doe", .password="hidden", .cash = 3.14, .karma = 42 };

   std::stringstream stream{};
   stream << out::sql::insert_into(u);

   REQUIRE(stream.str() == R"(INSERT INTO user (name,password,cash,karma) VALUES ('John Doe','hidden',3.14,42);)");
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}
