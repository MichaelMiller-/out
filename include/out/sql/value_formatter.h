#pragma once

#include <out/sql/value_formatter_fwd.h>

#include <iomanip>
#include <ostream>

namespace out::sql
{
   template <typename T, typename>
   struct value_formatter
   {
      static void apply(std::ostream& os, T const& value) { os << value; }
   };

   template <>
   struct value_formatter<std::string>
   {
      static void apply(std::ostream& os, std::string const& value) { os << std::quoted(value, '\''); }
   };
}
