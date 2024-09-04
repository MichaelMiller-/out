#pragma once

#include <type_traits>

namespace out
{
   template <typename T>
      requires std::is_class_v<T>
   class json_object;
}
