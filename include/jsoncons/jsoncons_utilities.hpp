// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSONCONS_UTILITIES_HPP
#define JSONCONS_JSONCONS_UTILITIES_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <ostream>
#include <cmath>
#include <algorithm> // std::find, std::min, std::reverse
#include <memory>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <istream> // std::basic_istream
#ifdef JSONCONS_HAS_VARIANT
#include <variant> // std::variant
#endif
#include <type_traits> // std::enable_if, std::true_type
#include <jsoncons/config/jsoncons_config.hpp>
#include <jsoncons/byte_string.hpp>
#include <jsoncons/json_exception.hpp>
#if !defined(JSONCONS_NO_TO_CHARS)
#include <charconv>
#endif

namespace jsoncons
{

// has_member_type_char_traits

template <class T, class CharT, class Enable=void>
struct has_member_type_char_traits : std::false_type {};

template <class T, class CharT>
struct has_member_type_char_traits<T, CharT, typename std::enable_if<std::is_same<typename T::traits_type::char_type, CharT>::value
>::type> : std::true_type {};

class string_viewable
{
#ifdef JSONCONS_HAS_VARIANT
    std::variant<std::string,jsoncons::string_view> v_;
#else
    jsoncons::string_view v_;
    std::string s_;
#endif
public:
    string_viewable() = default;

    string_viewable(const string_viewable&) = default;

    string_viewable(string_viewable&&) = default;

    string_viewable& operator=(const string_viewable&) = default;

    string_viewable& operator=(string_viewable&&) = default;

    template <class Source>
    string_viewable(const Source& source, typename std::enable_if<
                            std::is_same<typename std::decay<Source>::type,char*>::value ||
                            std::is_same<typename std::decay<Source>::type,const char*>::value>::type* = 0)
        : v_(jsoncons::string_view(source))
    {
    }
    template <class Source>
    string_viewable(const Source& source, typename std::enable_if<
                            std::is_same<typename std::decay<Source>::type,wchar_t*>::value ||
                            std::is_same<typename std::decay<Source>::type,const wchar_t*>::value 
                        >::type* = 0)
    {
    #ifdef JSONCONS_HAS_VARIANT
        std::string s_;
    #endif
        auto result = unicons::convert(
            source, source+std::wcslen(source), std::back_inserter(s_), unicons::conv_flags::strict);
        if (result.ec != unicons::conv_errc())
        {
            JSONCONS_THROW(json_exception_impl<std::runtime_error>("Encoding error"));
        }
    #ifdef JSONCONS_HAS_VARIANT
        v_ = s_;
    #endif
    }
    template <class Source>
    string_viewable(const Source& source, typename std::enable_if<
                            has_member_type_char_traits<typename std::decay<Source>::type,char>::value>::type* = 0)
        : v_(jsoncons::string_view(source.data(),source.length()))
    {
    }
    template <class Source>
    string_viewable(const Source& source, typename std::enable_if<
                            has_member_type_char_traits<typename std::decay<Source>::type,wchar_t>::value>::type* = 0)
    {
    #ifdef JSONCONS_HAS_VARIANT
        std::string s_;
    #endif
        auto result = unicons::convert(
            source.begin(), source.end(), std::back_inserter(s_), unicons::conv_flags::strict);
        if (result.ec != unicons::conv_errc())
        {
            JSONCONS_THROW(json_exception_impl<std::runtime_error>("Encoding error"));
        }
    #ifdef JSONCONS_HAS_VARIANT
        v_ = s_;
    #endif
    }

    jsoncons::string_view string_view() const
    {
    #ifdef JSONCONS_HAS_VARIANT
        return std::holds_alternative<std::string>(v_) ? std::get<0>(v_) : std::get<1>(v_);
    #else
        return v_.length() > 0 ? v_ : s_;
    #endif
    }

    operator jsoncons::string_view() const
    {
        return string_view();
    }
private:
};

template <class CharT>
class basic_null_istream : public std::istream
{
    class null_buffer : public std::basic_streambuf<CharT>
    {
    public:
        using typename std::basic_streambuf<CharT>::int_type;
        using typename std::basic_streambuf<CharT>::traits_type;
        int_type overflow( int_type ch = traits_type::eof() ) override
        {
            return ch;
        }
    } nb_;
public:
    basic_null_istream()
      : std::istream(&nb_)
    {
    }
};

static const char* base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                           "abcdefghijklmnopqrstuvwxyz"
                                           "0123456789+/"
                                           "=";
static const char* base64url_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                              "abcdefghijklmnopqrstuvwxyz"
                                              "0123456789-_"
                                              "\0";


template <class Container>
size_t encode_base16(const uint8_t* data, size_t length, Container& result)
{
    static const char* lut = "0123456789ABCDEF";

    for (size_t i = 0; i < length; ++i)
    {
        uint8_t c = data[i];
        result.push_back(lut[c >> 4]);
        result.push_back(lut[c & 15]);
    }
    return length*2;
}

template <class Container>
size_t encode_base64_generic(const uint8_t* first, size_t length, const char* alphabet, Container& result)
{
    size_t count = 0;
    const uint8_t* last = first + length;
    unsigned char a3[3];
    unsigned char a4[4];
    unsigned char fill = alphabet[64];
    int i = 0;
    int j = 0;

    while (first != last)
    {
        a3[i++] = *first++;
        if (i == 3)
        {
            a4[0] = (a3[0] & 0xfc) >> 2;
            a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
            a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
            a4[3] = a3[2] & 0x3f;

            for (i = 0; i < 4; i++) 
            {
                result.push_back(alphabet[a4[i]]);
                ++count;
            }
            i = 0;
        }
    }

    if (i > 0)
    {
        for (j = i; j < 3; ++j) 
        {
            a3[j] = 0;
        }

        a4[0] = (a3[0] & 0xfc) >> 2;
        a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
        a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);

        for (j = 0; j < i + 1; ++j) 
        {
            result.push_back(alphabet[a4[j]]);
            ++count;
        }

        if (fill != 0)
        {
            while (i++ < 3) 
            {
                result.push_back(fill);
                ++count;
            }
        }
    }

    return count;
}

template <class Container>
size_t encode_base64url(const uint8_t* first, size_t length, Container& result)
{
    return encode_base64_generic(first, length, base64url_alphabet, result);
}

template <class Container>
size_t encode_base64(const uint8_t* first, size_t length, Container& result)
{
    return encode_base64_generic(first, length, base64_alphabet, result);
}

inline 
static bool is_base64(uint8_t c) 
{
    return isalnum(c) || c == '+' || c == '/';
}

inline 
static bool is_base64url(uint8_t c) 
{
    return isalnum(c) || c == '-' || c == '_';
}

template <class F>
std::vector<uint8_t> decode_base64_generic(const std::basic_string<char>& base64_string, 
                                           const char* alphabet, 
                                           const char* alphabet_end, 
                                           F f)
{
    std::vector<uint8_t> result;
    uint8_t a4[4], a3[3];
    uint8_t i = 0;
    uint8_t j = 0;

    auto first = base64_string.begin();
    auto last = base64_string.end();

    while (first != last && *first != '=')
    {
        if (!f(*first))
        {
            throw std::invalid_argument("Invalid encoded string");
        }

        a4[i++] = *first++; 
        if (i == 4)
        {
            for (i = 0; i < 4; ++i) 
            {
                auto p = std::find(alphabet,alphabet_end,a4[i]);
                if (p == alphabet_end)
                {
                    a4[i] = 0xff;
                }
                else
                {
                    a4[i] = static_cast<uint8_t>(p - alphabet);
                }
            }

            a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
            a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
            a3[2] = ((a4[2] & 0x3) << 6) +   a4[3];

            for (i = 0; i < 3; i++) 
            {
                result.push_back(a3[i]);
            }
            i = 0;
        }
    }

    if (i > 0)
    {
        for (j = 0; j < i; ++j) 
        {
            auto p = std::find(alphabet,alphabet_end,a4[j]);
            if (p == alphabet_end)
            {
                a4[j] = 0xff;
            }
            else
            {
                a4[j] = static_cast<uint8_t>(p - alphabet);
            }
        }

        a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
        a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; ++j) 
        {
            result.push_back(a3[j]);
        }
    }

    return result;
}

inline
std::vector<uint8_t> decode_base64(const std::basic_string<char>& base64_string)
{
    return decode_base64_generic(base64_string, base64_alphabet, base64_alphabet+65, is_base64);
}

inline
std::vector<uint8_t> decode_base64url(const std::basic_string<char>& base64_string)
{
    return decode_base64_generic(base64_string, base64url_alphabet, base64url_alphabet+64, is_base64url);
}

inline
std::vector<uint8_t> decode_base16(const std::basic_string<char>& input)
{
    static const char* const alphabet = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) 
    {
        throw std::invalid_argument("odd length");
    }

    std::vector<uint8_t> result;
    result.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = (char)input[i];
        const char* p = std::lower_bound(alphabet, alphabet + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = (char)input[i + 1];
        const char* q = std::lower_bound(alphabet, alphabet + 16, b);
        if (*q != b) 
        {
            throw std::invalid_argument("not a hex digit");
        }

        result.push_back((uint8_t)(((p - alphabet) << 4) | (q - alphabet)));
    }
    return result;
}


// json_literals

namespace detail {

template <class CharT>
basic_string_view<CharT> null_literal()
{
    static const CharT chars[] = {'n','u','l', 'l'};
    return basic_string_view<CharT>(chars,sizeof(chars));
}

template <class CharT>
basic_string_view<CharT> true_literal()
{
    static const CharT chars[] = {'t','r','u', 'e'};
    return basic_string_view<CharT>(chars,sizeof(chars));
}

template <class CharT>
basic_string_view<CharT> false_literal()
{
    static const CharT chars[] = {'f','a','l', 's', 'e'};
    return basic_string_view<CharT>(chars,sizeof(chars));
}

}

}

#endif
