#include <catch.hpp>

#include <out/value_formatter.h>

TEST_CASE("format value of type: std::string", "[out]")
{
   std::stringstream stream;
   out::value_formatter<std::string>::apply(stream, "foobar");
   REQUIRE(stream.str() == "\"foobar\"");
}

TEST_CASE("format value of type: bool", "[out]")
{
   SECTION("value is 'true")
   {
      std::stringstream stream;
      out::value_formatter<bool>::apply(stream, true);
      REQUIRE(stream.str() == "true");
   }
   SECTION("value is 'false")
   {
      std::stringstream stream;
      out::value_formatter<bool>::apply(stream, false);
      REQUIRE(stream.str() == "false");
   }
}

TEST_CASE("format value of type: char", "[out]")
{
   std::stringstream stream;
   out::value_formatter<char>::apply(stream, 'a');
   REQUIRE(stream.str() == "97");
}

TEST_CASE("format value of type: unsigned char", "[out]")
{
   std::stringstream stream;
   out::value_formatter<unsigned char>::apply(stream, 'a');
   REQUIRE(stream.str() == "97");
}

TEST_CASE("format value of type: std::optional<int>", "[out]")
{
   SECTION("optional is empty")
   {
      std::stringstream stream;
      out::value_formatter<std::optional<int>>::apply(stream, std::nullopt);
      REQUIRE(stream.str() == "null");
   }
   SECTION("optional with arbitrary value")
   {
      std::stringstream stream;
      out::value_formatter<std::optional<int>>::apply(stream, 42);
      REQUIRE(stream.str() == "42");
   }
   SECTION("optional with string value")
   {
      std::stringstream stream;
      out::value_formatter<std::optional<std::string>>::apply(stream, "string");
      REQUIRE(stream.str() == "\"string\"");
   }
}

TEST_CASE("format value of type: std::filesystem::path", "[out]")
{
   std::stringstream stream;
   out::value_formatter<std::filesystem::path>::apply(stream, "/usr/bin");
   REQUIRE(stream.str() == "\"/usr/bin\"");
}
