#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <source_location>
extern "C" {
int APS5_VABI snprintf_nid_postfix(char*, std::size_t, const char*, ...);
int APS5_VABI sprintf_nid_postfix(char*, const char*, ...);
int APS5_VABI sscanf_nid_postfix(const char*, const char*, ...);
int APS5_VABI libc_printf_nid_postfix(const char*, ...);
}
static void Require(bool value, std::source_location location = std::source_location::current()) {
    if (!value) { std::fprintf(stderr, "Guest variadic formatting failed at line %u\n", location.line()); std::abort(); }
}
int main() {
    char buffer[128];
    Require(snprintf_nid_postfix(buffer, sizeof(buffer), "%d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7, 8) == 15);
    Require(std::strcmp(buffer, "1 2 3 4 5 6 7 8") == 0);
    Require(snprintf_nid_postfix(buffer, sizeof(buffer), "%d %d %d %d %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %s",
        1, 2, 3, 4, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, "end") == 47);
    Require(std::strcmp(buffer, "1 2 3 4 1.5 2.5 3.5 4.5 5.5 6.5 7.5 8.5 9.5 end") == 0);
    Require(snprintf_nid_postfix(buffer, sizeof(buffer), "%s %s %s %s %c%c", "a", "bb", "ccc", "dddd", 'x', 'y') == 16);
    Require(std::strcmp(buffer, "a bb ccc dddd xy") == 0);
    Require(snprintf_nid_postfix(buffer, sizeof(buffer), "%lld %lld %lld %lld", -1LL, 4294967297LL, -4294967297LL, 9007199254740993LL) == 42);
    Require(std::strcmp(buffer, "-1 4294967297 -4294967297 9007199254740993") == 0);
    std::memset(buffer, '!', sizeof(buffer));
    Require(snprintf_nid_postfix(buffer, 8, "%d %d %d %d %d", 1, 2, 3, 4, 5) == 9);
    Require(std::strcmp(buffer, "1 2 3 4") == 0 && buffer[8] == '!');
    Require(sprintf_nid_postfix(buffer, "%d %d %d %d %d %d", 1, 2, 3, 4, 5, 6) == 11);
    Require(std::strcmp(buffer, "1 2 3 4 5 6") == 0);
    int first = 0, second = 0, third = 0, fourth = 0, fifth = 0, sixth = 0;
    char word[16] = {};
    double real = 0.0;
    Require(sscanf_nid_postfix("1 2 3 4 5 6 seven 8.5", "%d %d %d %d %d %d %15s %lf",
        &first, &second, &third, &fourth, &fifth, &sixth, word, &real) == 8);
    Require(first == 1 && second == 2 && third == 3 && fourth == 4 && fifth == 5 && sixth == 6);
    Require(std::strcmp(word, "seven") == 0 && real == 8.5);
    Require(libc_printf_nid_postfix("%d %d %d %d %d %d %d\n", 1, 22, 333, 4444, 55555, 666666, 7777777) == 35);
    std::puts("Guest variadic formatting checks passed");
}
