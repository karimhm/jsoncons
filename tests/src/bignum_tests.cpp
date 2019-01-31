// Copyright 2013 Daniel Parker
// Distributed under Boost license

#if defined(_MSC_VER)
#include "windows.h"
#endif
#include <jsoncons/bignum.hpp>
#include <jsoncons/json.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <doctest/doctest.h>

using namespace jsoncons;

TEST_CASE("test_positive_bignum")
{
    std::string expected = "18446744073709551616";
    std::vector<uint8_t> v = {1,0,0,0,0,0,0,0,0};
    bignum x(1,v.data(),v.size());

    std::string sx;
    x.dump(sx);
    CHECK(expected == sx);

    bignum y(x);
    std::string sy;
    y.dump(sy);
    CHECK(expected == sy);

    bignum z;
    z = x;
    std::string sz;
    y.dump(sz);
    CHECK(expected == sz);
}

TEST_CASE("bignums are equal")
{
    std::string s = "18446744073709551616";
    bignum x(s);
    bignum y(s);

    bool test = x == y;
    CHECK(test);
}

TEST_CASE("test_negative_bignum")
{
    std::string expected = "-18446744073709551617";
    std::vector<uint8_t> b = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    bignum x(-1,b.data(),b.size());

    std::string sx;
    x.dump(sx);
    CHECK(expected == sx);

    bignum y(x);
    std::string sy;
    y.dump(sy);
    CHECK(expected == sy);

    bignum z;
    z = x;
    std::string sz;
    y.dump(sz);
    CHECK(expected == sz);

    int signum;
    std::vector<uint8_t> v;
    x.dump(signum, v);

    REQUIRE(v.size() == b.size());
    for (size_t i = 0; i < v.size(); ++i)
    {
        REQUIRE(v[i] == b[i]);
    }
}

TEST_CASE("test_longlong")
{
    long long n = (std::numeric_limits<long long>::max)();

    bignum val = n;

    //std::cout << "long long " << n << " == " << val << std::endl;
    //std::cout << val.to_string(16) << std::endl;
}

TEST_CASE("test_bignum2")
{
    std::string v = "10000000000000000";
    bignum val(v.data());

    //std::cout << val << std::endl;
}

TEST_CASE("test_logical_operations")
{
    bignum x( "888888888888888888" );
    bignum y( "888888888888888888" );

    bignum z = x & y;

    bool test = z == x;
    CHECK(test);
}

TEST_CASE("test_addition")
{
    bignum x( "4444444444444444444444444444444" );
    bignum y( "4444444444444444444444444444444" );
    bignum a( "8888888888888888888888888888888" );

    bignum z = x + y;
    bool test = z == a;
    CHECK(test);
}

TEST_CASE("test_multiplication")
{
    bignum x( "4444444444444444444444444444444" );
    bignum a( "8888888888888888888888888888888" );

    bignum z = 2*x;
    bool test = z == a;
    CHECK(test);

    z = x*2;

    test = z == a;
    CHECK(test);
}

TEST_CASE("test_conversion_0")
{
    bignum x(1, {});

    json j(x);

    bignum y = j.as<bignum>();
    CHECK(bool(x == y));

    std::string s;
    y.dump(s);

    CHECK(s == "0");
}

TEST_CASE("test_traits1")
{
    bignum x(1, {0x01,0x00});

    json j(x);

    bignum y = j.as<bignum>();
    CHECK(bool(x == y));

    std::string s;
    y.dump(s);

    CHECK(s == "256");
}

TEST_CASE("test_traits2")
{
    bignum x(1, {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00});

    json j(x);

    bignum y = j.as<bignum>();
    CHECK(bool(x == y));

    std::string s;
    y.dump(s);

    CHECK(s == "18446744073709551616");
}

TEST_CASE("test_traits3")
{
    bignum x(-1, {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00});

    int signum;
    std::vector<uint8_t> v;
    x.dump(signum,v);

    REQUIRE(signum == -1);
    //for (auto c : v)
    //{
    //    //std::cout << std::hex << (int)c;
    //}
    //std::cout << std::endl;

    json j(x);

    bignum y = j.as<bignum>();
    CHECK(bool(x == y));

    std::string s;
    y.dump(s);

    CHECK(s == "-18446744073709551617");
}

TEST_CASE("test shift left")
{
    SUBCASE("n << 1")
    {
        bignum n("1");
        bignum x = n << 1;
        std::string s;
        x.dump(s);
        CHECK(s == "2");
    }
    SUBCASE("n << 100")
    {
        bignum n(1);
        bignum x = n << 100;
        std::string s;
        x.dump(s);
        CHECK(s == "1267650600228229401496703205376");
    }
    SUBCASE("n << 100, += 1")
    {
        bignum n(1);
        bignum x = n << 100;
        x += 1;
        std::string s;
        x.dump(s);
        CHECK(s == "1267650600228229401496703205377");
    }
}

TEST_CASE("times 10")
{
    SUBCASE("1")
    {
        bignum n("1234");
        bignum m = n * 10;
        std::string s;
        m.dump(s);
        CHECK(s == "12340");
    }
    SUBCASE("31")
    {
        std::string expected("1234");
        bignum n(expected);

        for (size_t i = 0; i < 31; ++i)
        {
            n *= (uint64_t)10;
            expected.push_back('0');
        }
        std::string s;
        n.dump(s);
        CHECK(s == expected);
        //std::cout << "x31: " << s << "\n";
    }
    SUBCASE("32")
    {
        std::string expected("1234");
        bignum n(expected);
        for (size_t i = 0; i < 32; ++i)
        {
            n *= (uint64_t)10;
            expected.push_back('0');
        }
        std::string s;
        n.dump(s);
        CHECK(s == expected);
        //std::cout << "x31: " << s << "\n";
    }
}

