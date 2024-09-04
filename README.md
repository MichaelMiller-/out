# out

***out*** is a small header-only library to output arbitrary data-structures in various formats without additional serializer code. The [Reflection TS](https://en.cppreference.com/w/cpp/experimental/reflect) is used for this purpose.

## Overview

```cpp
#include <out/csv.h>
#include <out/json.h>

#include <iostream>
#include <string>
#include <vector>

struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{};
};

int main()
{
   std::vector<user> users{
      { "John Doe", "secret", 42, 3.14 },
      { "Max Mustermann", "****", 1, 45'678 }
   };

   std::cout << out::csv(users) << std::endl;
   std::cout << "---" << std::endl;
   std::cout << out::json(*begin(users)) << std::endl;
}
```

The output is:
```bash
name, password, karma, cash
John Doe, secret, 42, 3.14
Max Mustermann, ****, 1, 45678
---
{"name": "John Doe","password": "secret","karma": 42,"cash": 3.14}
```


## Supported compiler

Currently, only the [Clang compiler](https://gcc.godbolt.org/z/sns8e3Eoo) supports the [Reflection TS](https://en.cppreference.com/w/cpp/experimental/reflect) in a branch. I have tested the following compilers with **out**. 


| Compiler      | Version | Works      |
|---------------|---------|------------|
| GCC           | 13.2    | &cross;    |
|            | 14.1    | &cross;    |
| Clang         | 17.0    | &cross;    |
|          | 17.2    | &cross;    |
| MSVC (cl.exe) | 19.33   | &cross;    |
| Intel         | -       | not tested |



## Output formats
---
### CSV
*#include [<out/csv.h>](include/out/csv.h)*

Write a [sequence-container](https://en.cppreference.com/w/cpp/container) of arbitrary objects as [CSV](https://en.wikipedia.org/wiki/Comma-separated_values) formatted to a stream. 

---
### Json
*#include [<out/json.h>](include/out/json.h)*

With the help of this header, any object can be outputted as [JSON](https://en.wikipedia.org/wiki/JSON). A number of data types are supported.

- [arithmetic types](https://en.cppreference.com/w/cpp/types/is_arithmetic)
- [enum](https://en.cppreference.com/w/c/language/enum)
- [std::string](https://en.cppreference.com/w/cpp/string/basic_string)
- [std::optional](https://en.cppreference.com/w/cpp/utility/optional)
- [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path)
- [Sequence containers](https://en.cppreference.com/w/cpp/container)

If a special format for a custom type is desired. The [json_serializer](include/out/json_serializer.h) class should be specialised. 

For example, your data structure has a [boost::uuid](https://www.boost.org/doc/libs/1_81_0/libs/uuid/doc/uuid.html) as a member and you want to display this as a string in the JSON object. Then the template specialisation could look like this:

```cpp
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <iomanip>
#include <out/json.h>

template <>
struct out::json_serializer<boost::uuids::uuid>
{
    static void apply(std::ostream& os, boost::uuids::uuid const& value)
    {
        os << std::quoted(to_string(value));
    }
};

struct color
{
    boost::uuids::uuid id{};
    std::string name{};
};

int main()
{
    std::cout << out::json(color{}) << std::endl;
    return 0;
}
```

---
### SQL Statements
*#include [<out/sql/insert_into.h>](include/out/sql/insert_into.h)*

Generate SQL statements with the help of the [Reflection TS](https://en.cppreference.com/w/cpp/experimental/reflect). The [INSERT](https://www.w3schools.com/sql/sql_insert.asp) statement was implemented as the first example. 
A [customisation point](include/out/sql/value_formatter.h) for own types is also implemented here. The functional principle is analogous to the [json_serializer](include/out/json_serializer.h).

```cpp
struct user
{
   std::string name{};
   std::string password{};
   double cash{};
   int karma{};
};

int main()
{
    user obj{ .name="John Doe", .password="hidden", .cash = 3.14, .karma = 42 };
    std::cout << out::sql::insert_into(obj) << std::endl;
    return 0;
}
// prints: 
// INSERT INTO user (name,password,cash,karma) VALUES ('John Doe','hidden',3.14,42);
```

## enum_to_string
*#include [<out/enum_to_string.h>](include/out/enum_to_string.h)*

There is a small helper to get rid of tedious code that every one of us has written many times. A ```to_string``` function for every enumeration. Basically the code looks like this:  

```cpp
enum class DaysOfTheWeek { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

[[nodiscard]] constexpr auto to_string(DaysOfTheWeek value) -> std::string_view
{
   if (value == DaysOfTheWeek::Monday) {
    return "Monday";
   }
   // missing control path DaysOfTheWeek::Tuesday!
   if (value == DaysOfTheWeek::Wednesday) {
    return "Wednesday";
   }
   if (value == DaysOfTheWeek::Thursday) {
    return "Thurdsay"; // typo!
   }
   if (value == DaysOfTheWeek::Friday) {
    return "Friday";
   }
   if (value == DaysOfTheWeek::Saturday) {
    return "Saturday";
   }
   if (value == DaysOfTheWeek::Sunday) {
    return "Sunday";
   }
   // of course this control path can never be used, but the compiler warnings you about a missing return value.
   return "There is obvious no meaningful value!";
}

int main()
{
    auto value = DaysOfTheWeek::Thursday;
    std::cout << "value: " << to_string(value) << std::endl;
}
```
It is pretty clear that this code has some issues with typo's, extensibility and missing or invalid control paths.
The helper function ```ènum_to_string``` takes care of all this problems. It generates a compile-time-lookup table using reflection of all elements in the enumeration and return name of the element.  
Simply include the header and use the functions ```to_string``` and ```to_enum```. 

```cpp
<out/enum_to_string.h>

enum class DaysOfTheWeek { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

int main()
{
    auto value = DaysOfTheWeek::Thursday;
    std::cout << "value: " << out::to_string(value) << std::endl;
}
```

Live example on Compiler Explorer: https://gcc.godbolt.org/z/xvvzen5zr