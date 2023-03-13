#pragma once

#include <tuple>
#include <string_view>
#include <sstream>

namespace out
{
   template <typename... Ts, typename UnaryOperation>
   [[nodiscard]] auto concat(std::tuple<Ts...> const& tuple, std::string_view delimiter, UnaryOperation unary_op)
   {
      std::stringstream stream{};
      std::size_t index{0};

      const auto worker = [&](auto const& value) {
         if (index++ > 0) {
            stream << delimiter;
         }
         stream << unary_op(value);
      };

      std::apply([&](auto const&... value) { (worker(value), ...); }, tuple);

      return stream.str();
   }

   template <typename... Ts>
   [[nodiscard]] auto concat(std::tuple<Ts...> const& tuple, std::string_view delimiter = ", ")
   {
      return concat(tuple, delimiter, [](auto const& value) noexcept { return value; });
   }
}