#pragma once

#include <array>
#include <tuple>
#include <utility>

#if __has_include(<experimental/reflect>)
//! \todo #if __cpp_lib_reflection >= 201902

#include <experimental/reflect>

namespace out
{
   template <template <typename> typename Transform, std::experimental::reflect::ObjectSequence Sequence, auto... Is>
   consteval auto make_object_sequence_array(std::index_sequence<Is...>) noexcept
   {
      return std::array{Transform<std::experimental::reflect::get_element_t<Is, Sequence>>::value...};
   }

   template <std::experimental::reflect::ObjectSequence Sequence, auto... Is>
   consteval auto make_object_sequence_tuple(std::index_sequence<Is...>) noexcept
   {
      return std::tuple<std::experimental::reflect::get_element_t<Is, Sequence>...>{};
   }

   template <template <typename> typename Transform, std::experimental::reflect::ObjectSequence Sequence_t, auto... Is>
   consteval auto make_object_sequence_tuple(std::index_sequence<Is...>) noexcept
   {
      return std::tuple{Transform<std::experimental::reflect::get_element_t<Is, Sequence_t>>::value...};
   }
} // namespace out

#endif