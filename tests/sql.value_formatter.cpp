#include <catch.hpp>

#include <out/sql/value_formatter.h>

TEST_CASE("format value of type: std::string", "[out]")
{
   std::stringstream stream;
   out::sql::value_formatter<std::string>::apply(stream, "foobar");
   REQUIRE(stream.str() == "\'foobar\'");
}
