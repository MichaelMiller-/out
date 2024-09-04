#pragma once

#include <out/enum_to_string.h>
#include <out/json_object_fwd.h>
#include <out/json_serializer_fwd.h>

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <optional>
#include <ostream>
#include <type_traits>

namespace out
{
   template <typename T, typename>
   struct json_serializer
   {
      static void apply(std::ostream& os, T const& value)
      {
         if constexpr (std::is_class_v<T>) {
            os << json_object{value};
         } else {
            os << value;
         }
      }
   };

   template <typename T>
      requires std::is_enum_v<T>
   struct json_serializer<T>
   {
      static void apply(std::ostream& os, T const& value) { os << to_string(value); }
   };

   template <>
   struct json_serializer<std::string>
   {
      static void apply(std::ostream& os, std::string const& value) { os << std::quoted(value); }
   };

   template <>
   struct json_serializer<bool>
   {
      static void apply(std::ostream& os, bool value) { os << std::boolalpha << value; }
   };

   template <>
   struct json_serializer<char>
   {
      static void apply(std::ostream& os, char value) { os << static_cast<int>(value); }
   };

   template <>
   struct json_serializer<unsigned char>
   {
      static void apply(std::ostream& os, unsigned char value) { os << static_cast<int>(value); }
   };

   template <typename T>
   struct json_serializer<std::optional<T>>
   {
      static void apply(std::ostream& os, std::optional<T> const& value)
      {
         if (value == std::nullopt) {
            os << "null";
         } else {
            json_serializer<T>::apply(os, *value);
         }
      }
   };

   template <>
   struct json_serializer<std::filesystem::path>
   {
      static void apply(std::ostream& os, std::filesystem::path const& value)
      {
         json_serializer<decltype(value.string())>::apply(os, value.string());
      }
   };

   template <typename T>
   struct json_serializer<
      T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()), typename T::value_type>>
   {
      static void apply(std::ostream& os, T const& container)
      {
         os << '[';
         std::for_each(begin(container), end(container), [index{0}, &os](auto const& elem) mutable {
            if (index++ > 0) {
               os << ",";
            }
            json_serializer<typename T::value_type>::apply(os, elem);
         });
         os << ']';
      }
   };
} // namespace out
