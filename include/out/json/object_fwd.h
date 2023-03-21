#pragma once

#include <type_traits>

namespace out::json
{
   template <typename T>
      requires std::is_class_v<T>
   class object;
}
