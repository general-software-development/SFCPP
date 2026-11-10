#pragma once

// TODO: Finish

#include "pointers.hpp"
#include "buffers.hpp"
#include <sfcpp/arch/cpu.hpp>
#include <sfcpp/nortti.hpp>

namespace mm {
    class Arena;

    namespace internal {
        struct ArenaPoolSlotMeta {
            sfcpp::arch::flag_t used = 0;    // Ignore 'used' for merge blocks
            sfcpp::arch::flag_t merged = 0;  // If this is 1, treat this as an extension of the next block!
        };

        class BaseArenaPool {
            public:
            virtual sfcpp::arch::u64 used() const noexcept;
            virtual sfcpp::arch::u64 available() const noexcept;
            virtual bool can_allocate(sfcpp::arch::u64 count) const noexcept;
            virtual mem::sptr<char> allocate(sfcpp::arch::u64 count) noexcept;
        };
    }

    class ArenaPool : public internal::BaseArenaPool {
        private:
        mem::bounded_buffer<internal::ArenaPoolSlotMeta> metastruct = mem::bounded_buffer<internal::ArenaPoolSlotMeta>::uninitialised();
        mem::bounded_buffer<sfcpp::arch::u8> data = mem::bounded_buffer<sfcpp::arch::u8>::uninitialised();
        sfcpp::arch::u64 index = 0;
        void free(void* location);

        public:
        ArenaPool(sfcpp::arch::u64 size);
        sfcpp::arch::u64 used() const noexcept;
        sfcpp::arch::u64 available() const noexcept;
        bool can_allocate(sfcpp::arch::u64 count) const noexcept;
        mem::sptr<char> allocate(sfcpp::arch::u64 count) noexcept;
    };

    class Arena {
        private:
        ArenaPool pool0;
        ArenaPool pool1;

        public:
        Arena();

        sfcpp::arch::u64 used() const noexcept;
        sfcpp::arch::u64 available() const noexcept;
        bool can_allocate(sfcpp::arch::u64 count) const noexcept;
        mem::sptr<char> allocate(sfcpp::arch::u64 count) noexcept;
    };

    namespace internal {
        struct ArenaNode {
            Arena* prev;
            Arena* next;
            Arena value;
        };
    }
};
