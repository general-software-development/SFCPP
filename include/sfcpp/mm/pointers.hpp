#include <cstddef>
#include <cstdlib>
#include <sfcpp/arch/iint.hpp>
#include <sfcpp/internal.hpp>
#include <functional>
#include <new>

// These are not reimplementations of STL smart pointers.
// Do not treat them as such.

namespace mem {
    namespace internal {
        struct control {
            void* original;
            std::function<void(void*)> freer;
            sfcpp::arch::u64 references = 1;
        };
        inline static void standard_ptr_freer(void* data) {
            free(data);
        }
    }

    template <typename T>
    class ptr {
        private:
        void* data = NULL;
        std::function<void(void*)> freer = NULL;

        public:
        // TODO: Implement all of these
        inline ptr(void* data) {
            this->data = data;
            this->freer = internal::standard_ptr_freer;
        };
        inline ptr(void* data, std::function<void(void*)> freer) {
            this->data = data;
            this->freer = freer;
        };
        inline ptr<T> operator+(const sfcpp::arch::u64& other) const {
            return ptr<T>((T*)this->data + other, this->freer);
        };
        inline ptr<T> operator+(ptr<T>& other) const {
            return ptr<T>((T*)this->data + (sfcpp::arch::u64)other.get_data(), this->freer);
        };
        inline ptr operator-(const sfcpp::arch::u64& other) const {
            return ptr((T*)this->data - other, this->freer);
        };
        inline std::ptrdiff_t operator-(ptr<T>& other) const {
            return (T*)this->data - (T*)other.get_data();
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
            return *reinterpret_cast<T*>((T*)this->data+i);
        }
        inline const T& operator[](sfcpp::arch::u64 i) const {
            return *reinterpret_cast<T*>((T*)this->data+i);
        }
    };

    template <typename T>
    class sptr {
        private:
        void* data;
        internal::control* owner = nullptr;
        inline void release() noexcept {
            if (!owner)
                return;
            if (--owner->references == 0) {
                internal::control* c = owner;
                c->freer(owner->original);
                c->~control();
                std::free(c);
            }
            owner = nullptr;
            data = nullptr;
        };

        public:
        inline sptr(void* data): sptr(data, std::free) {};
        inline sptr(void* data, std::function<void(void*)> freer) {
            if (data == NULL) {
                _s::critical_err("Memory Error", "Attempted to initialise safe pointer (mem::sptr) from a null pointer.");
            }
        
            try {
                owner = (internal::control*)malloc(sizeof(internal::control));
                if (owner == NULL) {
                    _s::critical_err("Allocation Error", "Failed to allocate %llu bytes", sizeof(internal::control));
                }
                new (owner) internal::control{data, freer, 1};
                this->data = data;
            } catch (...) {
                std::free(owner);
                freer(data);
                throw;
            }
        };
        inline sptr(const sptr& other) noexcept : data(other.data), owner(other.owner) {
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
            n.data = (T*)this->data + other;
            return n;
        };
        inline sptr<T> operator+(sptr& other) const {
            return *this + (sfcpp::arch::u64)other.get_data();
        };
        inline sptr<T> operator-(const sfcpp::arch::u64& other) const {
            sptr<T> n = sptr<T>(*this);
            n.data = (T*)this->data - other;
            return n;
        };
        inline std::ptrdiff_t operator-(sptr<T>& other) const {
            return (T*)this->data - (T*)other.get_data();
        };

        inline void* get_data() {
            return this->data;
        }

        inline T& operator*() {
            return *(T*)this->data;
        }
        inline T& operator[](sfcpp::arch::u64 i) {
            return *reinterpret_cast<T*>((T*)this->data+i);
        }
        inline const T& operator[](sfcpp::arch::u64 i) const {
            return *reinterpret_cast<T*>((T*)this->data+i);
        }
    };

    //class borrowed_ptr;
}
