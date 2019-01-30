// Copyright 2019 Daniel Parker
// Distributed under Boost license

#include <jsoncons/json_exception.hpp>
#include <jsoncons/jsoncons_utilities.hpp>
#include <catch/catch.hpp>
#include <iostream>
#include <cstdio>

using namespace jsoncons;

TEST_CASE("string_viewable tests")
{
    SECTION("char literal")
    {
        string_viewable sv("foo");
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("const char*")
    {
        const char* s = "foo";
        string_viewable sv(s);
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("wchar_t literal")
    {
        string_viewable sv(L"foo");
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("const char*")
    {
        const wchar_t* s = L"foo";
        string_viewable sv(s);
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("std::string")
    {
        std::string s("foo");
        string_viewable sv(s);
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("const std::string")
    {
        const std::string s("foo");
        string_viewable sv(s);
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("std::wstring")
    {
        std::wstring s(L"foo");
        string_viewable sv(s);
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("const std::wstring")
    {
        const std::wstring s(L"foo");
        string_viewable sv(s);
        CHECK((sv.string_view() == "foo"));
    }
    SECTION("string_view")
    {
        jsoncons::string_view s("foo");
        string_viewable sv(s);
        CHECK((sv.string_view() == "foo"));
    }
}

