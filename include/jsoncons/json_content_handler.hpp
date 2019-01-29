// Copyright 2018 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSON_CONTENT_HANDLER_HPP
#define JSONCONS_JSON_CONTENT_HANDLER_HPP

#include <string>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/jsoncons_utilities.hpp>
#include <jsoncons/bignum.hpp>
#include <jsoncons/serializing_context.hpp>
#include <jsoncons/json_options.hpp>

namespace jsoncons {

// null_type

struct null_type
{
};

enum class semantic_tag_type : uint8_t 
{
    none = 0,
    undefined,
    date_time,
    timestamp,
    big_integer,
    big_decimal,
    base16,
    base64,
    base64url,
    uri,
    big_float // unused
};

class json_content_handler
{
public:
    typedef char char_type;
    typedef std::char_traits<char_type> char_traits_type;

    typedef basic_string_view<char_type,char_traits_type> string_view_type;

    virtual ~json_content_handler() {}

    void flush()
    {
        do_flush();
    }

    bool begin_object(semantic_tag_type tag=semantic_tag_type::none,
                      const serializing_context& context=null_serializing_context())
    {
        return do_begin_object(tag, context);
    }

    bool begin_object(size_t length, 
                      semantic_tag_type tag=semantic_tag_type::none, 
                      const serializing_context& context = null_serializing_context())
    {
        return do_begin_object(length, tag, context);
    }

    bool end_object(const serializing_context& context = null_serializing_context())
    {
        return do_end_object(context);
    }

    bool begin_array(semantic_tag_type tag=semantic_tag_type::none,
                     const serializing_context& context=null_serializing_context())
    {
        return do_begin_array(tag, context);
    }

    bool begin_array(size_t length, 
                     semantic_tag_type tag=semantic_tag_type::none,
                     const serializing_context& context=null_serializing_context())
    {
        return do_begin_array(length, tag, context);
    }

    bool end_array(const serializing_context& context=null_serializing_context())
    {
        return do_end_array(context);
    }

    bool name(const string_view_type& name, const serializing_context& context=null_serializing_context())
    {
        return do_name(name, context);
    }

    bool string_value(const string_view_type& value, 
                      semantic_tag_type tag = semantic_tag_type::none, 
                      const serializing_context& context=null_serializing_context()) 
    {
        return do_string_value(value, tag, context);
    }

    bool byte_string_value(const byte_string_view& b, 
                           semantic_tag_type tag=semantic_tag_type::none, 
                           const serializing_context& context=null_serializing_context())
    {
        return do_byte_string_value(b, tag, context);
    }

    bool byte_string_value(const uint8_t* p, size_t size, 
                           semantic_tag_type tag=semantic_tag_type::none, 
                           const serializing_context& context=null_serializing_context())
    {
        return do_byte_string_value(byte_string(p, size), tag, context);
    }

    bool big_integer_value(const string_view_type& s, const serializing_context& context=null_serializing_context()) 
    {
        return do_string_value(s, semantic_tag_type::big_integer, context);
    }

    bool big_decimal_value(const string_view_type& s, const serializing_context& context=null_serializing_context()) 
    {
        return do_string_value(s, semantic_tag_type::big_decimal, context);
    }

    bool date_time_value(const string_view_type& s, const serializing_context& context=null_serializing_context()) 
    {
        return do_string_value(s, semantic_tag_type::date_time, context);
    }

    bool timestamp_value(int64_t val, const serializing_context& context=null_serializing_context()) 
    {
        return do_int64_value(val, semantic_tag_type::timestamp, context);
    }

    bool int64_value(int64_t value, 
                     semantic_tag_type tag = semantic_tag_type::none, 
                     const serializing_context& context=null_serializing_context())
    {
        return do_int64_value(value, tag, context);
    }

    bool uint64_value(uint64_t value, 
                      semantic_tag_type tag = semantic_tag_type::none, 
                      const serializing_context& context=null_serializing_context())
    {
        return do_uint64_value(value, tag, context);
    }

    bool double_value(double value, 
                      semantic_tag_type tag = semantic_tag_type::none, 
                      const serializing_context& context=null_serializing_context())
    {
        return do_double_value(value, tag, context);
    }

    bool bool_value(bool value, 
                    semantic_tag_type tag = semantic_tag_type::none,
                    const serializing_context& context=null_serializing_context()) 
    {
        return do_bool_value(value, tag, context);
    }

    bool null_value(semantic_tag_type tag = semantic_tag_type::none,
                    const serializing_context& context=null_serializing_context()) 
    {
        return do_null_value(tag, context);
    }

private:
    virtual void do_flush() = 0;

    virtual bool do_begin_object(semantic_tag_type, const serializing_context& context) = 0;

    virtual bool do_begin_object(size_t, semantic_tag_type tag, const serializing_context& context)
    {
        return do_begin_object(tag, context);
    }

    virtual bool do_end_object(const serializing_context& context) = 0;

    virtual bool do_begin_array(semantic_tag_type, const serializing_context& context) = 0;

    virtual bool do_begin_array(size_t, semantic_tag_type tag, const serializing_context& context)
    {
        return do_begin_array(tag, context);
    }

    virtual bool do_end_array(const serializing_context& context) = 0;

    virtual bool do_name(const string_view_type& name, const serializing_context& context) = 0;

    virtual bool do_null_value(semantic_tag_type, const serializing_context& context) = 0;

    virtual bool do_string_value(const string_view_type& value, semantic_tag_type tag, const serializing_context& context) = 0;

    virtual bool do_byte_string_value(const byte_string_view& b, 
                                      semantic_tag_type tag, 
                                      const serializing_context& context) = 0;

    virtual bool do_double_value(double value, 
                                 semantic_tag_type tag,
                                 const serializing_context& context) = 0;

    virtual bool do_int64_value(int64_t value, 
                                semantic_tag_type tag,
                                const serializing_context& context) = 0;

    virtual bool do_uint64_value(uint64_t value, 
                                 semantic_tag_type tag, 
                                 const serializing_context& context) = 0;

    virtual bool do_bool_value(bool value, semantic_tag_type tag, const serializing_context& context) = 0;
};

class null_json_content_handler final : public json_content_handler
{
public:
    using typename json_content_handler::string_view_type;
private:
    void do_flush() override
    {
    }

    bool do_begin_object(semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_end_object(const serializing_context&) override
    {
        return true;
    }

    bool do_begin_array(semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_end_array(const serializing_context&) override
    {
        return true;
    }

    bool do_name(const string_view_type&, const serializing_context&) override
    {
        return true;
    }

    bool do_null_value(semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_string_value(const string_view_type&, semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_byte_string_value(const byte_string_view&,
                              semantic_tag_type, 
                              const serializing_context&) override
    {
        return true;
    }

    bool do_int64_value(int64_t, 
                        semantic_tag_type, 
                        const serializing_context&) override
    {
        return true;
    }

    bool do_uint64_value(uint64_t, 
                         semantic_tag_type, 
                         const serializing_context&) override
    {
        return true;
    }

    bool do_double_value(double, 
                         semantic_tag_type,
                         const serializing_context&) override
    {
        return true;
    }

    bool do_bool_value(bool, semantic_tag_type, const serializing_context&) override
    {
        return true;
    }
};

}

#endif
