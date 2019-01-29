// Copyright 2017 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CONVERSION_TRAITS_HPP
#define JSONCONS_CONVERSION_TRAITS_HPP

#include <string>
#include <tuple>
#include <array>
#include <memory>
#include <type_traits> // std::enable_if
#include <jsoncons/json_content_handler.hpp>
#include <jsoncons/json_decoder.hpp>
#include <jsoncons/basic_json.hpp>
#include <jsoncons/json_options.hpp>
#include <jsoncons/json_serializer.hpp>
#include <jsoncons/jsoncons_utilities.hpp>
#include <jsoncons/json_type_traits.hpp>
#include <jsoncons/staj_reader.hpp>

namespace jsoncons {


template <class T>
void decode_stream(staj_reader& reader, T& val, std::error_code& ec);

template <class T>
void decode_stream(staj_reader& reader, T& val)
{
    std::error_code ec;
    decode_stream(reader, val, ec);
    if (ec)
    {
        throw serialization_error(ec, reader.context().line_number(), reader.context().column_number());
    }
}

template <class T>
void encode_stream(const T&val, json_content_handler& writer);

} // namespace jsoncons

#include <jsoncons/staj_iterator.hpp>

namespace jsoncons {

template <class T, class Enable = void>
struct conversion_traits
{
    static T decode(staj_reader& reader, std::error_code& ec)
    {
        json_decoder<json> decoder;
        reader.accept(decoder, ec);
        return decoder.get_result().template as<T>();
    }

    static void encode(const T& val, json_content_handler& writer)
    {
        auto j = json_type_traits<json, T>::to_json(val);
        j.dump(writer);
    }
};

// specializations

// vector like

template <class T>
struct conversion_traits<T,
    typename std::enable_if<jsoncons::detail::is_vector_like<T>::value
>::type>
{
    typedef typename T::value_type value_type;

    static T decode(staj_reader& reader, std::error_code& ec)
    {
        T v;
        basic_staj_array_iterator<value_type> end;
        basic_staj_array_iterator<value_type> it(reader, ec);

        while (it != end && !ec)
        {
            v.push_back(*it);
            it.increment(ec);
        }
        return v;
    }

    static void encode(const T& val, json_content_handler& writer)
    {
        writer.begin_array();
        for (auto it = std::begin(val); it != std::end(val); ++it)
        {
            conversion_traits<value_type>::encode(*it,writer);
        }
        writer.end_array();
        writer.flush();
    }
};
// std::array

template <class T, size_t N>
struct conversion_traits<std::array<T,N>>
{
    typedef typename std::array<T,N>::value_type value_type;

    static std::array<T, N> decode(staj_reader& reader, std::error_code& ec)
    {
        std::array<T,N> v;
        v.fill(T{});
        basic_staj_array_iterator<value_type> end;
        basic_staj_array_iterator<value_type> it(reader, ec);

        for (size_t i = 0; it != end && i < N && !ec; ++i)
        {
            v[i] = *it;
            it.increment(ec);
        }
        return v;
    }

    static void encode(const std::array<T, N>& val, json_content_handler& writer)
    {
        writer.begin_array();
        for (auto it = std::begin(val); it != std::end(val); ++it)
        {
            conversion_traits<value_type>::encode(*it,writer);
        }
        writer.end_array();
        writer.flush();
    }
};

// map like

template <class T>
struct conversion_traits<T,
    typename std::enable_if<jsoncons::detail::is_map_like<T>::value
>::type>
{
    typedef typename T::mapped_type mapped_type;
    typedef typename T::value_type value_type;
    typedef typename T::key_type key_type;

    static T decode(staj_reader& reader, std::error_code& ec)
    {
        
        T m;
        basic_staj_object_iterator<mapped_type> end;
        basic_staj_object_iterator<mapped_type> it(reader, ec);

        while (it != end && !ec)
        {
            m.emplace(it->first,it->second);
            it.increment(ec);
        }
        return m;
    }

    static void encode(const T& val, json_content_handler& writer)
    {
        writer.begin_object();
        for (auto it = std::begin(val); it != std::end(val); ++it)
        {
            writer.name(it->first);
            conversion_traits<mapped_type>::encode(it->second,writer);
        }
        writer.end_object();
        writer.flush();
    }
};

template <class T>
void decode_stream(staj_reader& reader, T& val, std::error_code& ec)
{
    val = conversion_traits<T>::decode(reader,ec);
}

template <class T>
void encode_stream(const T&val, json_content_handler& writer)
{
    conversion_traits<T>::encode(val, writer);
}

}

#endif

