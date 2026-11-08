#pragma once

using TypeId = const void*;

namespace nortti {
    template <typename T>
    inline TypeId type_id() noexcept {
        static const char id = 0;
        return &id;
    }
}
