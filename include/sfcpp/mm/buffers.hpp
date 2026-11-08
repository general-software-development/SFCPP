#include <sfcpp/internal.hpp>
#include <sfcpp/arch/iint.hpp>
#include <sfcpp/mm/pointers.hpp>

namespace mem {
    template <sfcpp::arch::u64 size, typename T>
    class static_bounded_buffer {
        private:
        mem::sptr<T> data;

        public:
        inline static_bounded_buffer() {
            this->data = mem::sptr<T>(std::calloc(size, sizeof(T)), free);  // TODO: Replace with arena
        };
        inline static_bounded_buffer(mem::sptr<T> data) {
            this->data = data;
        };

        inline const T& operator[](sfcpp::arch::u64 i) const {
            static_assert(size > i);
            if (i >= size)
                _s::critical_err("Memory Error", "Attempted to dereference item %zu of static_bounded_buffer<%zu> (address %p)", i, size, this->data.get_data() + i);
            return this->data[i];
        };

        inline T& operator[](sfcpp::arch::u64 i) {
            static_assert(size > i);
            if (i >= size)
                _s::critical_err("Memory Error", "Attempted to dereference item %zu of static_bounded_buffer<%zu> (address %p)", i, size, this->data.get_data() + i);
            return this->data[i];
        };

        inline T* get_data() {
            return this->data;
        };
    };

    template <typename T>
    class bounded_buffer {
        private:
        mem::sptr<T> data;
        sfcpp::arch::u64 size;

        public:
        inline bounded_buffer(sfcpp::arch::u64 size) {
            this->size = size;
            this->data = mem::sptr<T>(std::calloc(size, sizeof(T)), free);  // TODO: Replace with arena
        };
        inline bounded_buffer(sfcpp::arch::u64 size, mem::sptr<T> data) {
            this->size = size;
            this->data = data;
        };

        inline const T& operator[](sfcpp::arch::u64 i) const {
            static_assert(this->size > i);
            if (i >= this->size)
                _s::critical_err("Memory Error", "Attempted to dereference item %zu of static_bounded_buffer<%zu> (address %p)", i, this->size, this->data.get_data() + i);
            return this->data[i];
        };

        inline T& operator[](sfcpp::arch::u64 i) {
            static_assert(this->size > i);
            if (i >= this->size)
                _s::critical_err("Memory Error", "Attempted to dereference item %zu of static_bounded_buffer<%zu> (address %p)", i, this->size, this->data.get_data() + i);
            return this->data[i];
        };

        inline bounded_buffer<T>& operator=(const bounded_buffer<T>& other) {
            this->data = other.data;
            this->size = other.size;
        }

        inline bounded_buffer<T>& operator=(bounded_buffer<T>&& other) {
            this->data = other.data;
            this->size = other.size;
            other.data.release();
            other.size = 0;
        }

        inline T* get_data() {
            return this->data;
        };
    };
};
