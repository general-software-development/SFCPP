#include <cstdio>
#include <cstdarg>
#include <stdexcept>
#include <sfcpp/arch/iint.hpp>

namespace _s {
    [[noreturn]]
    void critical_err(const sfcpp::arch::uchar* errtype, const sfcpp::arch::uchar* fmt, ...) {
        va_list vl;
        va_start(vl, fmt);
        fprintf(stderr, "SFCPP: %s - ", (const char*)errtype);
        vfprintf(stderr, (const char*)fmt, vl);
        fprintf(stderr, "\n");
        va_end(vl);
        *((volatile unsigned char*)0x0);

        throw std::runtime_error((const char*)fmt);
    }

    [[noreturn]]
    void critical_err(const char* errtype, const char* fmt, ...) {
        va_list vl;
        va_start(vl, fmt);
        fprintf(stderr, "SFCPP: %s - ", errtype);
        vfprintf(stderr, fmt, vl);
        fprintf(stderr, "\n");
        va_end(vl);
        *((volatile unsigned char*)0x0);

        throw std::runtime_error(fmt);
    }
}
