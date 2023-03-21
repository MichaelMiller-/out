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
| Clang         | 12.2    | &cross;    |
| GCC           | 15.0    | &cross;    |
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

With the help of this header, any object can be output as [JSON](https://en.wikipedia.org/wiki/JSON). A number of data types are supported.

- [arithmetic types](https://en.cppreference.com/w/cpp/types/is_arithmetic)
- [enum](https://en.cppreference.com/w/c/language/enum)
- [std::string](https://en.cppreference.com/w/cpp/string/basic_string)
- [std::optional](https://en.cppreference.com/w/cpp/utility/optional)
- [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path)
- [Sequence containers](https://en.cppreference.com/w/cpp/container)

If a special format for a custom type is desired. The [value_formatter](include/out/json/value_formatter.h) class should be specialised. 

For example, your data structure has a [boost::uuid](https://www.boost.org/doc/libs/1_81_0/libs/uuid/doc/uuid.html) as a member and you want to display this as a string in the JSON object. Then the template specialisation could look like this:

```cpp
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <iomanip>
#include <out/json.h>

template <>
struct out::json::value_formatter<boost::uuids::uuid>
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
A [customisation point](include/out/sql/value_formatter.h) for own types is also implemented here. The functional principle is analogous to the [json/value_formatter](include/out/json/value_formatter.h).

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


