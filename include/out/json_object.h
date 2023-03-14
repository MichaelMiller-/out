#pragma once

#if __has_include(<experimental/reflect>)
// #if __cpp_lib_reflection >= 201902

#include <out/make_object_sequence.h>
#include <out/value_formatter.h>

#include <experimental/reflect>
#include <string>
#include <type_traits>

namespace out
{
   template <typename T>
      requires std::is_class_v<T>
   class json_object
   {
      T obj;
      std::size_t index{0};
      std::string file_content{};

      template <typename Member>
      auto write_member(std::ostream& os, Member)
      {
         if (index++ > 0) {
            os << ",";
         }
         os << std::quoted(std::experimental::reflect::get_name_v<Member>) << ": ";
         auto value = obj.*std::experimental::reflect::get_pointer_v<Member>;
         value_formatter<decltype(value)>::apply(os, value);
      }

      auto generate_content() -> std::string
      {
         namespace reflect = std::experimental::reflect;

         using reflection_t = reflexpr(T);
         using members_t = reflect::get_public_data_members_t<reflection_t>;

         constexpr auto members =
            make_object_sequence_tuple<members_t>(std::make_index_sequence<reflect::get_size_v<members_t>>{});

         std::stringstream stream{};
         stream << '{';
         std::apply([&](auto const&... member) { (write_member(stream, member), ...); }, members);
         stream << '}';
         return stream.str();
      }

    public:
      explicit json_object(T object) : obj{std::move(object)}, file_content{generate_content()} {}

      [[nodiscard]] auto content() const noexcept { return file_content; }
   };

   template <typename T>
   std::ostream& operator<<(std::ostream& os, json_object<T> const& obj)
   {
      return os << obj.content();
   }
} // namespace out

#endif