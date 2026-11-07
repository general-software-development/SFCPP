#include <cstddef>
#include <cstdlib>
#include <sfcpp/arch/iint.hpp>
#include <sfcpp/internal.hpp>

// These are not reimplementations of STL smart pointers.
// Do not treat them as such.

namespace mem {
    namespace internal {
        struct control {
            void* original;
            void (*freer)(void*);
            sfcpp::arch::u64 references = 1;
        };
        void standard_ptr_freer(void* data) {
            free(data);
        }
    }

    template <typename T>
    class ptr {
        private:
        void* data = NULL;
        void(*freer)(void* data) = NULL;

        public:
        // TODO: Implement all of these
        inline ptr(void* data) {
            this->data = data;
            this->freer = internal::standard_ptr_freer;
        };
        inline ptr(void* data, void(*freer)(void* data)) {
            this->data = data;
            this->freer = freer;
        };
        inline ptr<T> operator+(const sfcpp::arch::u64& other) const {
            return ptr<T>((char*)this->data + other);
        };
        inline ptr<T> operator+(ptr<T>& other) const {
            return ptr<T>((char*)this->data + (sfcpp::arch::u64)other.get_data());
        };
        inline ptr operator-(const sfcpp::arch::u64& other) const {
            return ptr((char*)this->data - other);
        };
        inline sfcpp::arch::u64 operator-(ptr& other) const {
            return (char*)this->data - (char*)other.get_data();
        };
        inline void free() {
            this->freer(this->data);
        };

        inline void* get_data() {
            return this->data;
        }
        inline T& operator*() {
            return *reinterpret_cast<T*>(this->data);
        }
        inline const T& operator*() const {
            return *reinterpret_cast<T*>(this->data);
        }
        inline T& operator[](sfcpp::arch::u64 i) {
            return *reinterpret_cast<T*>(this->data+i);
        }
        inline const T& operator*(sfcpp::arch::u64 i) const {
            return *reinterpret_cast<T*>(this->data+i);
        }
    };

    template <typename T>
    class sptr {
        private:
        void* data;
        internal::control* owner = nullptr;
        void release() noexcept;

        public:
        inline sptr(void* data): sptr(data, free) {};
        inline sptr(void* data, void(*freer)(void* data)) {
            if (data == NULL) {
                _s::critical_err("Memory Error", "Attempted to initialise safe pointer (mem::sptr) from a null pointer.");
            }
        
            try {
                owner = (internal::control*)malloc(sizeof(internal::control));
                if (owner == NULL) {
                    _s::critical_err("Allocation Error", "Failed to allocate %llu bytes", sizeof(internal::control));
                }
                owner->original = data;
                owner->freer = freer;
                owner->references = 1;
            } catch (...) {
                freer(data);
                throw;
            }
        };
        inline sptr(const sptr& other) noexcept {
            if (owner)
                owner->references++;
        };
        inline sptr(sptr&& other) noexcept : data(other.data), owner(other.owner) {
            other.data = NULL;
            other.owner = NULL;
        };
        inline ~sptr() {
            release();
        };

        inline sptr& operator=(const sptr& other) noexcept {
            if (this == &other)
                return *this;

            release();

            data = other.data;
            owner = other.owner;

            if (owner)
                ++owner->references;

            return *this;
        };
        inline sptr& operator=(sptr&& other) noexcept {
            if (this == &other)
                return *this;

            release();

            data = other.data;
            owner = other.owner;

            other.data = NULL;
            other.owner = NULL;

            return *this;
        };

        inline sptr<T> operator+(const sfcpp::arch::u64& other) const {
            sptr<T> n = sptr<T>(*this);
            n.owner = this.owner;
            n.data = this.data + other;
        };
        inline sptr operator+(sptr& other) const {
            return *this + (sfcpp::arch::u64)other.get_data();
        };
        inline sptr operator-(const sfcpp::arch::u64& other) const {
            sptr<T> n = sptr<T>(*this);
            n.owner = this.owner;
            n.data = this.data - other;
        };
        inline sfcpp::arch::u64 operator-(sptr& other) const {
            return this->data - other.get_data();
        };

        inline void* get_data() {
            return this->data;
        }

        inline T& operator*() {
            return *this->data;
        }
        inline T& operator[](sfcpp::arch::u64 i) {
            return *reinterpret_cast<T*>(this->data+i);
        }
        inline const T& operator*(sfcpp::arch::u64 i) const {
            return *reinterpret_cast<T*>(this->data+i);
        }
    };

    //class borrowed_ptr;
}
