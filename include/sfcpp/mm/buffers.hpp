#include <sfcpp/arch/iint.hpp>

#ifndef SFCPP_INPUT_STATIC_BOUNDED_BUFFER_INIT
#define SFCPP_INPUT_STATIC_BOUNDED_BUFFER_INIT template class static_bounded_buffer<64, int>;
#endif

namespace mem {
    // THE CALLER IS RESPONSIBLE FOR FREEING MEMOERY
    template <sfcpp::arch::u64 size, typename T>
    class static_bounded_buffer {
        private:
        T* data;  // TODO: Replace with sptr

        public:
        static_bounded_buffer();
        static_bounded_buffer(T* data);

        const T& operator[](sfcpp::arch::u64 i) const;

        T& operator[](sfcpp::arch::u64 i);

        T* get_data();
    };

    SFCPP_INPUT_STATIC_BOUNDED_BUFFER_INIT
};
