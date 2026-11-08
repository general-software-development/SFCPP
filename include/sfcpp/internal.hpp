#pragma once

#include <sfcpp/arch/iint.hpp>

namespace _s {
    [[noreturn]]
    void critical_err(const sfcpp::arch::uchar* errtype, const sfcpp::arch::uchar* fmt, ...);

    [[noreturn]]
    void critical_err(const char* errtype, const char* fmt, ...);
}
