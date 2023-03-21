#pragma once

#include <out/make_object_sequence.h>
#include <out/sql/value_formatter_fwd.h>

#include <experimental/reflect>
#include <iomanip>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>

// https://compiler-explorer.com/z/b6jEK3xx3

namespace out::sql
{
   template <typename T>
   class insert_into_statement
   {
      T obj;
      std::size_t index{0};
      std::string statement{};

      template <typename Member>
      auto format_value(std::ostream& os, Member)
      {
         if (index++ > 0) {
            os << ",";
         }
         auto value = obj.*std::experimental::reflect::get_pointer_v<Member>;
         value_formatter<decltype(value)>::apply(os, value);
      }

      auto generate_statement() -> std::string
      {
         namespace reflect = std::experimental::reflect;

         using reflection_t = reflexpr(T);
         using aliased_t = std::experimental::reflect::get_aliased_t<reflection_t>;
         using members_t = reflect::get_public_data_members_t<reflection_t>;

         constexpr auto members =
            make_object_sequence_tuple<members_t>(std::make_index_sequence<reflect::get_size_v<members_t>>{});
         constexpr auto member_names =
            std::apply([]<typename... Ts>(Ts...) { return std::make_tuple(reflect::get_name_v<Ts>...); }, members);

         std::stringstream stream{};
         stream << "INSERT INTO " << reflect::get_name_v<aliased_t> << " (" << concat(member_names, ",") << ") VALUES (";
         std::apply([&](auto const&... member) { (format_value(stream, member), ...); }, members);
         stream << ");";
         return stream.str();
      }

    public:
      explicit insert_into_statement(T object) : obj{std::move(object)}, statement{generate_statement()} {}

      [[nodiscard]] auto content() const noexcept { return statement; }
   };

   template <typename T>
   auto& operator<<(std::ostream& os, insert_into_statement<T> const& obj)
   {
      return os << obj.content();
   }

   template <typename T>
      requires std::is_class_v<T>
   inline auto insert_into(T const& value)
   {
      return insert_into_statement{value};
   }
} // namespace out::sql
