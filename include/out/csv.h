#pragma once

#include <concepts>
#include <string>

#if __has_include(<experimental/reflect>)
//! \todo #if __cpp_lib_reflection >= 201902

#include <out/concat.h>
#include <out/make_object_sequence.h>

namespace out
{
   class csv_file
   {
      std::string file_content{};

      template <typename T, typename Iterator>
      auto generate_content(Iterator first, Iterator last) noexcept -> std::string
      {
         namespace reflect = std::experimental::reflect;

         if (std::distance(first, last) == 0) {
            return {};
         }

         using reflection_t = reflexpr(T);
         using members_t = reflect::get_public_data_members_t<reflection_t>;

         constexpr auto members =
            make_object_sequence_tuple<members_t>(std::make_index_sequence<reflect::get_size_v<members_t>>{});
         constexpr auto member_names =
            std::apply([]<typename... Ts>(Ts...) { return std::make_tuple(reflect::get_name_v<Ts>...); }, members);
         constexpr auto member_pointers =
            std::apply([]<typename... Ts>(Ts...) { return std::make_tuple(reflect::get_pointer_v<Ts>...); }, members);

         std::stringstream stream{};
         stream << concat(member_names, ", ") << std::endl;

         for (; first != last; ++first) {
            const auto values = std::apply(
               [value = *first](auto const&... member) { return std::make_tuple(value.*member...); }, member_pointers);
            stream << concat(values, ", ") << std::endl;
         }
         return stream.str();
      }

    public:
      template <typename Iterator>
      constexpr explicit csv_file(Iterator first, Iterator last)
          : file_content{generate_content<typename std::iterator_traits<Iterator>::value_type>(first, last)}
      {
      }

      [[nodiscard]] auto content() const noexcept { return file_content; }
   };

   std::ostream& operator<<(std::ostream& os, csv_file const& obj) { return os << obj.content(); }

   template <typename Iterator>
   [[nodiscard]] auto csv(Iterator first, Iterator last)
   {
      return csv_file{first, last};
   }

   [[nodiscard]] auto csv(std::ranges::range auto const& container)
   {
      return csv_file{begin(container), end(container)};
   }
} // namespace out

#endif