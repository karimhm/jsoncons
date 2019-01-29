// Copyright 2017 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSON_CONVERSION_HPP
#define JSONCONS_JSON_CONVERSION_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <istream> // std::basic_istream
#include <jsoncons/conversion_traits.hpp>
#include <jsoncons/json_staj_reader.hpp>

namespace jsoncons {

// decode_json

template <class T>
T decode_json(const std::basic_string<char>& s)
{
    json_staj_reader reader(s);
    T val;
    decode_stream(reader, val);
    return val;
}

template <class T>
T decode_json(const std::basic_string<char>& s,
              const json_options& options)
{
    json_staj_reader reader(s, options);
    T val;
    decode_stream(reader, val);
    return val;
}

template <class T>
T decode_json(std::istream& is)
{
    json_staj_reader reader(is);
    T val;
    decode_stream(reader, val);
    return val;
}

template <class T>
T decode_json(std::istream& is,
              const json_options& options)
{
    json_staj_reader reader(is, options);
    T val;
    decode_stream(reader, val);
    return val;
}

// encode_json

template <class T>
void encode_json(const T& val, json_content_handler& writer)
{
    encode_stream(val, writer);
    writer.flush();
}

template <class T>
void encode_json(const T& val, std::ostream& os)
{
    json_compressed_serializer serializer(os);
    encode_json(val, serializer);
}

template <class T>
void encode_json(const T& val, const json_options& options,
          std::ostream& os)
{
    json_compressed_serializer serializer(os, options);
    encode_json(val, serializer);
}

template <class T>
void encode_json(const T& val, std::ostream& os, indenting line_indent)
{
    if (line_indent == indenting::indent)
    {
        json_serializer serializer(os);
        encode_json(val, serializer);
    }
    else
    {
        json_compressed_serializer serializer(os);
        encode_json(val, serializer);
    }
}

template <class T>
void encode_json(const T& val, const json_options& options,
          std::ostream& os, indenting line_indent)
{
    if (line_indent == indenting::indent)
    {
        json_serializer serializer(os, options);
        encode_json(val, serializer);
    }
    else
    {
        json_compressed_serializer serializer(os, options);
        encode_json(val, serializer);
    }
}

template <class T>
void encode_json(const T& val, std::basic_string<char>& s)
{
    basic_json_compressed_serializer<char,jsoncons::string_result<std::basic_string<char>>> serializer(s);
    encode_json(val, serializer);
}

template <class T>
void encode_json(const T& val, const json_options& options,
          std::basic_string<char>& s)
{
    basic_json_compressed_serializer<char,jsoncons::string_result<std::basic_string<char>>> serializer(s, options);
    encode_json(val, serializer);
}

template <class T>
void encode_json(const T& val, std::basic_string<char>& s, indenting line_indent)
{
    if (line_indent == indenting::indent)
    {
        basic_json_serializer<char,jsoncons::string_result<std::basic_string<char>>> serializer(s);
        encode_json(val, serializer);
    }
    else
    {
        basic_json_compressed_serializer<char,jsoncons::string_result<std::basic_string<char>>> serializer(s);
        encode_json(val, serializer);
    }
}

template <class T>
void encode_json(const T& val, const json_options& options,
                 std::basic_string<char>& s, indenting line_indent)
{
    if (line_indent == indenting::indent)
    {
        json_serializer serializer(s, options);
        encode_json(val, serializer);
    }
    else
    {
        json_compressed_serializer serializer(s, options);
        encode_json(val, serializer);
    }
}

}

#endif

