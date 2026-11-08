// TODO: Finish

#include "pointers.hpp"
#include "buffers.hpp"
#include <sfcpp/arch/cpu.hpp>
#include <sfcpp/nortti.hpp>

namespace mm {
    class ArenaBase;

    namespace internal {
        template <typename T>
        struct ArenaNode {
            ArenaBase<T>* prev;
            ArenaBase<T>* next;
            ArenaBase<T> value;
        };
    }

    template <typename T>
    class ArenaPool {
        private:
        mem::bounded_buffer<T> memory = NULL;
        mem::bounded_buffer<sfcpp::arch::u64> occmap = NULL;
        mem::bounded_buffer<sfcpp::arch::u64> allocation_sizes = NULL;
        sfcpp::arch::u64 capacity = 0;
        sfcpp::arch::u64 used = 0;

        inline bool is_occupied(std::size_t i) const noexcept {
            return (occmap[i / 64] >> (i % 64)) & 1ULL;
        }

        void set_occupied(std::size_t i, bool value) noexcept {
            const auto mask = 1ULL << (i % 64);

            if (value)
                bitmap[i / 64] |= mask;
            else
                bitmap[i / 64] &= ~mask;
        }

        void reclaim_tail() noexcept {
            while (occupied > 0 && !is_occupied(occupied - 1))
                --occupied;
        }

        bool free(T* ptr, std::size_t count) noexcept {
            if (!ptr || !memory || !bitmap || count == 0)
                return false;

            // Find the allocation's start without subtracting
            // unrelated pointers.
            std::size_t start = 0;

            for (; start < occupied; ++start) {
                if (memory + start == ptr)
                    break;
            }

            if (start == occupied || count > occupied - start)
                return false;

            // Reject ranges containing already-freed slots.
            for (std::size_t i = 0; i < count; ++i) {
                if (!is_occupied(start + i))
                    return false;
            }

            for (std::size_t i = 0; i < count; ++i)
                set_occupied(start + i, false);

            reclaim_tail();
            return true;
        }

        public:
        ArenaPool() noexcept = default;
        
        explicit inline ArenaPool(mem::bounded_buffer<T> mem) {
            this->memory = mem;
            this->used = 0;
            this->capacity = mem.size;
            this->occmap = mem::bounded_buffer(mem.size);
            this->allocation_sizes = mem::bounded_buffer(mem.size);
        }

        inline sfcpp::arch::u64 used() noexcept const {
            return used;
        }

        inline sfcpp::arch::u64 available() noexcept const {
            return capacity - used;
        }

        inline bool can_allocate(sfcpp::arch::u64 count) const noexcept {
            return memory != NULL && count <= available();
        }

        inline mem::sptr<T> allocate(sfcpp::arch::u64 count) noexcept {
            if (!can_allocate(count))
                return NULL;

            T* result = memory + occupied;

            occupied += count;

            return mem::sptr(result, [](T*p){this->free(??)});
        }

        inline void reset() noexcept {
            occupied = 0;
        }
    };

    class ArenaBase {
        TypeId type;
    };

    template <typename T>
    class Arena : public ArenaBase {
        private:
        ArenaPool<T> pool0;
        ArenaPool<T> pool1;

        public:
        inline Arena() {
            type = type_id<T>();
        };

        mem::sptr<T> alloc() {

        }
    };
};
