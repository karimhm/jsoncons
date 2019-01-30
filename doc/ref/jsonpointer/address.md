### jsoncons::jsonpointer::address

```
template <class CharT>
class address
```
#### Header
```c++
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>
```

Objects of type `address` represent JSON Pointer addresses.

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

    address();

    explicit address(const string_type& s);

    explicit address(string_type&& s);

    explicit address(const CharT* s);

    address(const address&);

    address(address&&);

#### operator=

    address& operator=(const address&);

    address& operator=(address&&);

#### Modifiers

    address& operator/=(const string_type& s)
First, appends the JSON Pointer separator `/` to the path. Then appends the string token s, escaping any `/` or `~` characters.

    address& operator+=(const address& p)
Concatenates the current address and the specified address `p`. 

#### Iterators

    iterator begin() const;
    iterator end() const;
Iterator access to the tokens in the path.

#### Accessors

    bool empty() const
Checks if the address is empty

    const string_type& string() const
Returns the string representation of the JSON Pointer by reference.

    operator string_view_type() const;
Returns a string view representation of the JSON Pointer.

#### Non-member functions
    address operator/(const address& lhs, const string_type& rhs);
Concatenates a JSON Pointer address and a token. Effectively returns address(lhs) /= rhs.

    address operator+( const address& lhs, const address& rhs );
Concatenates two JSON Pointer paths. Effectively returns address(lhs) += rhs.

    bool operator==(const address& lhs, const address& rhs);

    bool operator!=(const address& lhs, const address& rhs);

    std::ostream&
    operator<<(std::ostream& os, const address& p);
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

