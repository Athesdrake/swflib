#pragma once
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

static void pt_context() { }
template <typename T> static void pt_context(T value) { std::cerr << value; }
template <typename T, typename... Args> static void pt_context(T value, Args... args) {
    std::cerr << value << ", ";
    pt_context(args...);
}

#define assert(value, ...)                                                                                             \
    {                                                                                                                  \
        if (!(value)) {                                                                                                \
            std::cerr << __FILE__ << ":" << __LINE__ << " In function '" << __PRETTY_FUNCTION__ << "':\n\t"            \
                      << "Assertation failed, expected a true value.\n\tContext:";                                     \
            pt_context(__VA_ARGS__);                                                                                   \
            std::cerr << "\n\n";                                                                                       \
            std::abort();                                                                                              \
        }                                                                                                              \
    }

#define assert_equal(value, expecting, ...)                                                                            \
    {                                                                                                                  \
        if ((value) != (expecting)) {                                                                                  \
            std::cerr << __FILE__ << ":" << __LINE__ << " In function '" << __PRETTY_FUNCTION__ << "':\n\t"            \
                      << "Assertation failed, expected `" << expecting << "` but got `" << value                       \
                      << "` instead.\n\tContext:";                                                                     \
            pt_context(__VA_ARGS__);                                                                                   \
            std::cerr << "\n\n";                                                                                       \
            std::abort();                                                                                              \
        }                                                                                                              \
    }

#define bench(expr, number)                                                                                            \
    {                                                                                                                  \
        using namespace std::chrono;                                                                                   \
        auto t1 = high_resolution_clock::now();                                                                        \
        for (size_t i = 0; i < number; ++i)                                                                            \
            volatile auto const X = expr;                                                                              \
                                                                                                                       \
        auto t2   = high_resolution_clock::now();                                                                      \
        auto took = duration_cast<milliseconds>(t2 - t1).count();                                                      \
        std::cout << #expr << " [" << number << "] took ";                                                             \
        std::cout << took << " ms.\n";                                                                                 \
    }

template <typename T> std::string inline hex(const T* array, size_t size) {
    static constexpr const char* HEXDIGITS = "0123456789abcdef";

    std::stringstream buf;
    for (auto i = 0; i < size; i++) {
        buf << HEXDIGITS[array[i] >> 4] << HEXDIGITS[array[i] & 0xf] << ' ';
    }

    auto s = buf.str();
    s.pop_back();
    return s;
}