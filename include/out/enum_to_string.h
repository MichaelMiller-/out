#pragma once

#if __has_include(<experimental/reflect>)
//! \todo #if __cpp_lib_reflection >= 201902

#include <algorithm>
#include <array>
#include <experimental/reflect>
#include <functional>
#include <string_view>

namespace out
{
   namespace detail
   {
      template <typename T>
         requires std::experimental::reflect::Constant<T> && std::experimental::reflect::Named<T>
      struct constant_and_name
      {
         static constexpr auto value =
            std::pair{std::experimental::reflect::get_constant_v<T>, std::experimental::reflect::get_name_v<T>};
      };

      template <typename T>
      consteval auto enum_lookup() noexcept
         requires std::is_enum_v<T>
      {
         using reflection_t = reflexpr(T);
         using enumerators_t = std::experimental::reflect::get_enumerators_t<reflection_t>;

         return []<auto... Is>(std::index_sequence<Is...>) {
            return std::array{
               detail::constant_and_name<std::experimental::reflect::get_element_t<Is, enumerators_t>>::value...};
         }(std::make_index_sequence<std::experimental::reflect::get_size_v<enumerators_t>>{});
      }
   } // namespace detail

   template <typename T>
   [[nodiscard]] constexpr auto to_string(T value) noexcept -> std::string_view
      requires std::is_enum_v<T>
   {
      constexpr auto values = detail::enum_lookup<T>();
      const auto it = std::find_if(begin(values), end(values), [value](auto const& e) { return e.first == value; });
      return it->second;
   }

   template <typename T, typename Compare = std::equal_to<>>
   [[nodiscard]] constexpr auto to_enum(std::string_view value) -> T
      requires std::is_enum_v<T>
   {
      constexpr auto values = detail::enum_lookup<T>();
      const auto it =
         std::find_if(begin(values), end(values), [value](auto const& e) { return Compare{}(e.second, value); });
      if (it == end(values)) {
         throw std::runtime_error("cannot convert string to enum");
      }
      return it->first;
   }
} // namespace out

#endif
