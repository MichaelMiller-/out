#pragma once

#include <out/json_object.h>

#include <type_traits>

namespace out
{
   template <typename T>
      requires std::is_class_v<T>
   [[nodiscard]] constexpr auto json(T const& value)
   {
      return json_object{value};
   }
} // namespace out
