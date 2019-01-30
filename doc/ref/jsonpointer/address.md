### jsoncons::jsonpointer::address

```
template <class CharT>
class path
```
#### Header
```c++
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>
```

Two specializations for common character types are defined:

Type      |Definition
----------|------------------------------
path      |`path<char>`
wpath     |`path<wchar_t>`

Objects of type `path` represent JSON Pointer strings.

#### Member types
Type        |Definition
------------|------------------------------
char_type   | `CharT`
string_type | `std::basic_string<char_type>`
string_view_type | `jsoncons::basic_string_view<char_type>`
const_iterator | A constant [LegacyInputIterator](https://en.cppreference.com/w/cpp/named_req/InputIterator) with a `value_type` of `std::basic_string<char_type>`
iterator    | An alias to `const_iterator`

#### Header
```c++
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>
```

#### Constructors

    path();

    explicit path(const string_type& s);

    explicit path(string_type&& s);

    explicit path(const CharT* s);

    path(const path&);

    path(path&&);

#### operator=

    path& operator=(const path&);

    path& operator=(path&&);

#### Modifiers

    path& operator/=(const string_type& s)
First, appends the JSON Pointer separator `/` to the path. Then appends the string token s, escaping any `/` or `~` characters.

    path& operator+=(const path& p)
Concatenates the current path and the specified path `p`. 

#### Iterators

    iterator begin() const;
    iterator end() const;
Iterator access to the tokens in the path.

#### Accessors

    bool empty() const
Checks if the path is empty

    const string_type& string() const
Returns the string representation of the JSON Pointer by reference.

    operator string_view_type() const;
Returns a string view representation of the JSON Pointer.

#### Non-member functions
    path operator/(const path& lhs, const string_type& rhs);
Concatenates a JSON Pointer path and a token. Effectively returns path(lhs) /= rhs.

    path operator+( const path& lhs, const path& rhs );
Concatenates two JSON Pointer paths. Effectively returns path(lhs) += rhs.

    bool operator==(const path& lhs, const path& rhs);

    bool operator!=(const path& lhs, const path& rhs);

    std::ostream&
    operator<<(std::ostream& os, const path& p);
Performs stream output

### Examples

#### Iterate over the tokens in a JSON Pointer

```c++
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>

namespace jp = jsoncons::jsonpointer;

int main()
{
    jp::address p("/store/book/1/author");

    std::cout << "(1) " << p << "\n\n";

    std::cout << "(2)\n";
    for (const auto& token : p)
    {
        std::cout << token << "\n";
    }

    std::cout << "\n";
}
```
Output:
```
(1) /store/book/1/author

(2)
store
book
1
author
```

#### Append tokens to a JSON Pointer

```c++
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>

namespace jp = jsoncons::jsonpointer;

int main()
{
    jp::address p;

    p /= "a/b";
    p /= "";
    p /= "m~n";

    std::cout << "(1) " << p << "\n\n";

    std::cout << "(2)\n";
    for (const auto& token : p)
    {
        std::cout << token << "\n";
    }

    std::cout << "\n";
}
```
Output:
```
(1) /a~1b//m~0n

(2)
a/b

m~n
```

#### Concatentate two JSON Pointers

```c++
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>

namespace jp = jsoncons::jsonpointer;

int main()
{
    jp::address p("/a~1b");

    p += jp::address("//m~0n");

    std::cout << "(1) " << p << "\n\n";

    std::cout << "(2)\n";
    for (const auto& token : p)
    {
        std::cout << token << "\n";
    }

    std::cout << "\n";
}
```
Output:
```
(1) /a~1b//m~0n

(2)
a/b

m~n
```

