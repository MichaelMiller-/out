#pragma once

#include <out/enum_to_string.h>
#include <out/json/object_fwd.h>
#include <out/json/value_formatter_fwd.h>

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <optional>
#include <ostream>
#include <type_traits>

namespace out::json
{
   template <typename T, typename>
   struct value_formatter
   {
      static void apply(std::ostream& os, T const& value)
      {
         if constexpr (std::is_class_v<T>) {
            os << object{value};
         } else {
            os << value;
         }
      }
   };

   template <typename T>
      requires std::is_enum_v<T>
   struct value_formatter<T>
   {
      static void apply(std::ostream& os, T const& value) { os << to_string(value); }
   };

   template <>
   struct value_formatter<std::string>
   {
      static void apply(std::ostream& os, std::string const& value) { os << std::quoted(value); }
   };

   template <>
   struct value_formatter<bool>
   {
      static void apply(std::ostream& os, bool value) { os << std::boolalpha << value; }
   };

   template <>
   struct value_formatter<char>
   {
      static void apply(std::ostream& os, char value) { os << static_cast<int>(value); }
   };

   template <>
   struct value_formatter<unsigned char>
   {
      static void apply(std::ostream& os, unsigned char value) { os << static_cast<int>(value); }
   };

   template <typename T>
   struct value_formatter<std::optional<T>>
   {
      static void apply(std::ostream& os, std::optional<T> const& value)
      {
         if (value == std::nullopt) {
            os << "null";
         } else {
            value_formatter<T>::apply(os, *value);
         }
      }
   };

   template <>
   struct value_formatter<std::filesystem::path>
   {
      static void apply(std::ostream& os, std::filesystem::path const& value)
      {
         value_formatter<decltype(value.string())>::apply(os, value.string());
      }
   };

   template <typename T>
   struct value_formatter<
      T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()), typename T::value_type>>
   {
      static void apply(std::ostream& os, T const& container)
      {
         os << '[';
         std::for_each(begin(container), end(container), [index{0}, &os](auto const& elem) mutable {
            if (index++ > 0) {
               os << ",";
            }
            value_formatter<typename T::value_type>::apply(os, elem);
         });
         os << ']';
      }
   };
} // namespace out::json
