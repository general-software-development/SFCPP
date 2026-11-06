//                                                                                                 |

#include <cstdlib>
#include <sfcpp/arch/iint.hpp>
#include <sfcpp/mm/buffers.hpp>

using namespace sfcpp::arch;

#define _mem_static_bounded_buffer_template template <sfcpp::arch::u64 size, typename T>

namespace mem {
    _mem_static_bounded_buffer_template
    static_bounded_buffer<size, T>::static_bounded_buffer() {
        this->data = static_cast<T*>(std::calloc(size, sizeof(T)));  // TODO: Replace with arena
    }

    _mem_static_bounded_buffer_template
    static_bounded_buffer<size, T>::static_bounded_buffer(T* data) {
        this->data = data;
    }

    _mem_static_bounded_buffer_template
    const T& static_bounded_buffer<size, T>::operator[](sfcpp::arch::u64 i) const {
        return this->data[i];
    }

    _mem_static_bounded_buffer_template
    T& static_bounded_buffer<size, T>::operator[](sfcpp::arch::u64 i) {
        return this->data[i];
    }

    _mem_static_bounded_buffer_template
    T* static_bounded_buffer<size, T>::get_data() {
        return this->data;
    }
}
