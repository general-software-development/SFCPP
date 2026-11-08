#include <sfcpp/mm/arenas.hpp>
#include <sfcpp/internal.hpp>

using namespace sfcpp::arch;

namespace mm {
    ArenaPool::ArenaPool(u64 size) {
        this->metastruct = mem::bounded_buffer<internal::ArenaPoolSlotMeta>(size);
        this->data = mem::bounded_buffer<u8>(size);
        this->index = 0;  // next free block
    }

    u64 ArenaPool::used() const noexcept {
        return this->index;
    }

    u64 ArenaPool::available() const noexcept {
        return this->data.get_size() - this->index;
    }

    bool ArenaPool::can_allocate(u64 count) const noexcept {
        return count <= this->available();
    }

    mem::sptr<char> ArenaPool::allocate(u64 count) noexcept {
        metastruct[index].used = 1;
        metastruct[index].merged = 0;
        for (u64 i = 1; i < count; i++) {
            metastruct[index+i].used = 0;
            metastruct[index+i].merged = 1;
        }
        index += count;
        return
        mem::sptr<char>(
            (this->data.get_data() + (index - count)).get_data(),
            [this](void* d){this->free(d);}
        );
    }

    void ArenaPool::free(void* location) {
        u64 locindex = (char*)location - (char*)data.get_data().get_data();
        if (metastruct[locindex].merged == 1) {
            _s::critical_err("Invariant Error", "ArenaPool -- block representing location being freed is a merge block.");
        }
        metastruct[locindex].used = 0;
        u64 i = 1;
        while (true) {
            if (metastruct[locindex + i].merged == 0)
                break;

            if (metastruct[locindex + i].used == 1)
                _s::critical_err("Invariant Error", "ArenaPool -- merge block has used=1");

            metastruct[locindex + i].merged = 0;
            i++;
        }

        for (u64 i = index - 1; i >= 0; i--) {
            if (metastruct[i].used == 0 && metastruct[i].merged == 0)
                index--;
        }
    }
}
