// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSON_OPTIONS_HPP
#define JSONCONS_JSON_OPTIONS_HPP

#include <string>
#include <limits> // std::numeric_limits
#include <cwchar>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/jsoncons_utilities.hpp>
#include <jsoncons/detail/type_traits_helper.hpp>

namespace jsoncons {

#if !defined(JSONCONS_NO_TO_CHARS)
using chars_format = std::chars_format;
#else
enum class chars_format : uint8_t {fixed=1,scientific=2,hex=4,general=fixed|scientific};
#endif

// floating_point_options

class floating_point_options
{
    chars_format format_;
    int precision_;
    uint8_t decimal_places_;
public:
    floating_point_options()
        : format_(chars_format::general), precision_(0), decimal_places_(0)
    {
    }

    floating_point_options(chars_format format, int precision, uint8_t decimal_places = 0)
        : format_(format), precision_(precision), decimal_places_(decimal_places)
    {
    }

    explicit floating_point_options(chars_format format)
        : format_(format), precision_(0), decimal_places_(0)
    {
    }

    floating_point_options(const floating_point_options&) = default;
    floating_point_options(floating_point_options&&) = default;
    floating_point_options& operator=(const floating_point_options& e) = default;
    floating_point_options& operator=(floating_point_options&& e) = default;

    int precision() const
    {
        return precision_;
    }

    uint8_t decimal_places() const
    {
        return decimal_places_;
    }

    chars_format format() const
    {
        return format_;
    }
};

enum class indenting : uint8_t {no_indent = 0, indent = 1};

enum class line_split_kind  : uint8_t {same_line,new_line,multi_line};

enum class big_integer_chars_format : uint8_t {number, base10, base64, base64url
};

enum class byte_string_chars_format : uint8_t {none=0,base16,base64,base64url};

enum class spaces_option{no_spaces=0,space_after,space_before,space_before_and_after};

class json_read_options
{
public:
    typedef std::basic_string<char> string_type;

    virtual ~json_read_options() = default;

    virtual size_t max_nesting_depth() const = 0;

    virtual bool is_str_to_nan() const = 0;

    virtual std::basic_string<char> nan_to_str() const = 0;

    virtual bool is_str_to_inf() const = 0;

    virtual std::basic_string<char> inf_to_str() const = 0;

    virtual bool is_str_to_neginf() const = 0;

    virtual std::basic_string<char> neginf_to_str() const = 0;

    virtual bool lossless_number() const = 0;
};

class json_write_options
{
public:
    typedef std::basic_string<char> string_type;

    virtual ~json_write_options() = default;

    virtual size_t max_nesting_depth() const = 0;

    virtual byte_string_chars_format byte_string_format() const = 0; 

    virtual big_integer_chars_format big_integer_format() const = 0; 

    virtual line_split_kind object_object_line_splits() const = 0; 

    virtual line_split_kind array_object_line_splits() const = 0; 

    virtual line_split_kind object_array_line_splits() const = 0; 

    virtual line_split_kind array_array_line_splits() const = 0; 

    virtual size_t  indent_size() const = 0;  

    virtual size_t line_length_limit() const = 0;  

    virtual chars_format floating_point_format() const = 0; 

    virtual int precision() const = 0; 

    virtual bool escape_all_non_ascii() const = 0; 

    virtual bool escape_solidus() const = 0; 

    virtual spaces_option spaces_around_colon() const = 0;  

    virtual spaces_option spaces_around_comma() const = 0;  

    virtual bool pad_inside_object_braces() const = 0;

    virtual bool pad_inside_array_brackets() const = 0;

    virtual std::basic_string<char> new_line_chars() const = 0;

    virtual bool is_nan_to_num() const = 0;

    virtual std::basic_string<char> nan_to_num() const = 0;

    virtual bool is_inf_to_num() const = 0;

    virtual std::basic_string<char> inf_to_num() const = 0;

    virtual bool is_neginf_to_num() const = 0;

    virtual std::basic_string<char> neginf_to_num() const = 0;

    virtual bool is_nan_to_str() const = 0;

    virtual std::basic_string<char> nan_to_str() const = 0;

    virtual bool is_inf_to_str() const = 0;

    virtual std::basic_string<char> inf_to_str() const = 0;

    virtual bool is_neginf_to_str() const = 0;

    virtual std::basic_string<char> neginf_to_str() const = 0;
};

class json_options : public virtual json_read_options, 
                     public virtual json_write_options
{
public:
    typedef char char_type;
    typedef std::basic_string<char> string_type;
private:
    size_t indent_size_;
    chars_format floating_point_format_;
    int precision_;
    bool escape_all_non_ascii_;
    bool escape_solidus_;
    byte_string_chars_format byte_string_format_;
    big_integer_chars_format big_integer_format_;
    line_split_kind object_object_line_splits_;
    line_split_kind object_array_line_splits_;
    line_split_kind array_array_line_splits_;
    line_split_kind array_object_line_splits_;
    size_t line_length_limit_;

    size_t max_nesting_depth_;
    spaces_option spaces_around_colon_;
    spaces_option spaces_around_comma_;
    bool pad_inside_object_braces_;
    bool pad_inside_array_brackets_;
    std::basic_string<char> new_line_chars_;

    bool is_nan_to_num_;
    bool is_inf_to_num_;
    bool is_neginf_to_num_;
    bool is_nan_to_str_;
    bool is_inf_to_str_;
    bool is_neginf_to_str_;
    bool is_str_to_nan_;
    bool is_str_to_inf_;
    bool is_str_to_neginf_;

    std::basic_string<char> nan_to_num_;
    std::basic_string<char> inf_to_num_;
    std::basic_string<char> neginf_to_num_;
    std::basic_string<char> nan_to_str_;
    std::basic_string<char> inf_to_str_;
    std::basic_string<char> neginf_to_str_;

    bool lossless_number_;
public:
    static const size_t indent_size_default = 4;
    static const size_t line_length_limit_default = 120;

//  Constructors

    json_options()
        : indent_size_(indent_size_default),
          floating_point_format_(chars_format()),
          precision_(0),
          escape_all_non_ascii_(false),
          escape_solidus_(false),
          byte_string_format_(byte_string_chars_format::none),
          big_integer_format_(big_integer_chars_format::base10),
          object_object_line_splits_(line_split_kind::multi_line),
          object_array_line_splits_(line_split_kind::same_line),
          array_array_line_splits_(line_split_kind::new_line),
          array_object_line_splits_(line_split_kind::multi_line),
          line_length_limit_(line_length_limit_default),
          max_nesting_depth_((std::numeric_limits<size_t>::max)()),
          spaces_around_colon_(spaces_option::space_after),
          spaces_around_comma_(spaces_option::space_after),
          pad_inside_object_braces_(false),
          pad_inside_array_brackets_(false),
          is_nan_to_num_(false),
          is_inf_to_num_(false),
          is_neginf_to_num_(false),
          is_nan_to_str_(false),
          is_inf_to_str_(false),
          is_neginf_to_str_(false),
          is_str_to_nan_(false),
          is_str_to_inf_(false),
          is_str_to_neginf_(false),
          lossless_number_(false)
    {
        new_line_chars_.push_back('\n');
    }

//  Properties
    byte_string_chars_format byte_string_format() const override {return byte_string_format_;}
    json_options&  byte_string_format(byte_string_chars_format value) {byte_string_format_ = value; return *this;}

    big_integer_chars_format big_integer_format() const override {return big_integer_format_;}
    json_options&  big_integer_format(big_integer_chars_format value) {big_integer_format_ = value; return *this;}

    line_split_kind object_object_line_splits() const override {return object_object_line_splits_;}
    json_options& object_object_line_splits(line_split_kind value) {object_object_line_splits_ = value; return *this;}

    line_split_kind array_object_line_splits() const override {return array_object_line_splits_;}
    json_options& array_object_line_splits(line_split_kind value) {array_object_line_splits_ = value; return *this;}

    line_split_kind object_array_line_splits() const override {return object_array_line_splits_;}
    json_options& object_array_line_splits(line_split_kind value) {object_array_line_splits_ = value; return *this;}

    line_split_kind array_array_line_splits() const override {return array_array_line_splits_;}
    json_options& array_array_line_splits(line_split_kind value) {array_array_line_splits_ = value; return *this;}

    size_t indent_size() const override
    {
        return indent_size_;
    }

    json_options& indent_size(size_t value)
    {
        indent_size_ = value;
        return *this;
    }

    spaces_option spaces_around_colon() const override
    {
        return spaces_around_colon_;
    }

    json_options& spaces_around_colon(spaces_option value)
    {
        spaces_around_colon_ = value;
        return *this;
    }

    spaces_option spaces_around_comma() const override
    {
        return spaces_around_comma_;
    }

    json_options& spaces_around_comma(spaces_option value)
    {
        spaces_around_comma_ = value;
        return *this;
    }

    bool pad_inside_object_braces() const override
    {
        return pad_inside_object_braces_;
    }

    json_options& pad_inside_object_braces(bool value)
    {
        pad_inside_object_braces_ = value;
        return *this;
    }

    bool pad_inside_array_brackets() const override
    {
        return pad_inside_array_brackets_;
    }

    json_options& pad_inside_array_brackets(bool value)
    {
        pad_inside_array_brackets_ = value;
        return *this;
    }

    std::basic_string<char> new_line_chars() const override
    {
        return new_line_chars_;
    }

    json_options& new_line_chars(const std::basic_string<char>& value)
    {
        new_line_chars_ = value;
        return *this;
    }

    bool is_nan_to_num() const override
    {
        return is_nan_to_num_;
    }

    bool is_inf_to_num() const override
    {
        return is_inf_to_num_;
    }

    bool is_neginf_to_num() const override
    {
        return is_neginf_to_num_ || is_inf_to_num_;
    }

    bool is_nan_to_str() const override
    {
        return is_nan_to_str_;
    }

    bool is_str_to_nan() const override
    {
        return is_str_to_nan_;
    }

    bool is_inf_to_str() const override
    {
        return is_inf_to_str_;
    }

    bool is_str_to_inf() const override
    {
        return is_str_to_inf_;
    }

    bool is_neginf_to_str() const override
    {
        return is_neginf_to_str_ || is_inf_to_str_;
    }

    bool is_str_to_neginf() const override
    {
        return is_str_to_neginf_ || is_str_to_inf_;
    }

    std::basic_string<char> nan_to_num() const override
    {
        if (is_nan_to_num_)
        {
            return nan_to_num_;
        }
        else
        {
            return nan_to_num_; // empty string
        }
    }

    json_options& nan_to_num(const std::basic_string<char>& value) 
    {
        is_nan_to_num_ = true;
        nan_to_str_.clear();
        nan_to_num_ = value;
        return *this;
    }

    std::basic_string<char> inf_to_num() const override
    {
        if (is_inf_to_num_)
        {
            return inf_to_num_;
        }
        else
        {
            return inf_to_num_; // empty string
        }
    }

    json_options& inf_to_num(const std::basic_string<char>& value) 
    {
        is_inf_to_num_ = true;
        inf_to_str_.clear();
        inf_to_num_ = value;
        return *this;
    }

    std::basic_string<char> neginf_to_num() const override
    {
        if (is_neginf_to_num_)
        {
            return neginf_to_num_;
        }
        else if (is_inf_to_num_)
        {
            std::basic_string<char> s;
            s.push_back('-');
            s.append(inf_to_num_);
            return s;
        }
        else
        {
            return neginf_to_num_; // empty string
        }
    }

    json_options& neginf_to_num(const std::basic_string<char>& value) 
    {
        is_neginf_to_num_ = true;
        neginf_to_str_.clear();
        neginf_to_num_ = value;
        return *this;
    }

    std::basic_string<char> nan_to_str() const override
    {
        if (is_nan_to_str_)
        {
            return nan_to_str_;
        }
        else
        {
            return nan_to_str_; // empty string
        }
    }

    json_options& nan_to_str(const std::basic_string<char>& value, bool is_str_to_nan = true) 
    {
        is_nan_to_str_ = true;
        is_str_to_nan_ = is_str_to_nan;
        nan_to_num_.clear();
        nan_to_str_ = value;
        return *this;
    }

    std::basic_string<char> inf_to_str() const override
    {
        if (is_inf_to_str_)
        {
            return inf_to_str_;
        }
        else
        {
            return inf_to_str_; // empty string
        }
    }

    json_options& inf_to_str(const std::basic_string<char>& value, bool is_inf_to_str = true) 
    {
        is_inf_to_str_ = true;
        is_inf_to_str_ = is_inf_to_str;
        inf_to_num_.clear();
        inf_to_str_ = value;
        return *this;
    }

    std::basic_string<char> neginf_to_str() const override
    {
        if (is_neginf_to_str_)
        {
            return neginf_to_str_;
        }
        else if (is_inf_to_str_)
        {
            std::basic_string<char> s;
            s.push_back('-');
            s.append(inf_to_str_);
            return s;
        }
        else
        {
            return neginf_to_str_; // empty string
        }
    }

    json_options& neginf_to_str(const std::basic_string<char>& value, bool is_neginf_to_str = true) 
    {
        is_neginf_to_str_ = true;
        is_neginf_to_str_ = is_neginf_to_str;
        neginf_to_num_.clear();
        neginf_to_str_ = value;
        return *this;
    }

    bool lossless_number() const override
    {
        return lossless_number_;
    }

    json_options& lossless_number(bool value) 
    {
        lossless_number_ = value;
        return *this;
    }

    size_t line_length_limit() const override
    {
        return line_length_limit_;
    }

    json_options& line_length_limit(size_t value)
    {
        line_length_limit_ = value;
        return *this;
    }

    chars_format floating_point_format() const override
    {
        return floating_point_format_;
    }

    json_options& floating_point_format(chars_format value)
    {
        floating_point_format_ = value;
        return *this;
    }

    int precision() const override
    {
        return precision_;
    }

    json_options& precision(int value)
    {
        precision_ = value;
        return *this;
    }

    bool escape_all_non_ascii() const override
    {
        return escape_all_non_ascii_;
    }

    json_options& escape_all_non_ascii(bool value)
    {
        escape_all_non_ascii_ = value;
        return *this;
    }

    bool escape_solidus() const override
    {
        return escape_solidus_;
    }

    json_options& escape_solidus(bool value)
    {
        escape_solidus_ = value;
        return *this;
    }

    size_t max_nesting_depth() const override
    {
        return max_nesting_depth_;
    }

    void max_nesting_depth(size_t value)
    {
        max_nesting_depth_ = value;
    }

private:
    enum class input_state {initial,begin_quote,character,end_quote,escape,error};
    bool is_string(const string_type& s) const
    {
        input_state state = input_state::initial;
        for (char_type c : s)
        {
            switch (c)
            {
            case '\t': case ' ': case '\n': case'\r':
                break;
            case '\\':
                state = input_state::escape;
                break;
            case '\"':
                switch (state)
                {
                case input_state::initial:
                    state = input_state::begin_quote;
                    break;
                case input_state::begin_quote:
                    state = input_state::end_quote;
                    break;
                case input_state::character:
                    state = input_state::end_quote;
                    break;
                case input_state::end_quote:
                    state = input_state::error;
                    break;
                case input_state::escape:
                    state = input_state::character;
                    break;
                default:
                    state = input_state::character;
                    break;
                }
                break;
            default:
                break;
            }

        }
        return state == input_state::end_quote;
    }
};

}
#endif
