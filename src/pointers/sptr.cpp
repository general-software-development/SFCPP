#include <sfcpp/arch/iint.hpp>
#include <src/internal.hpp>
#include <sfcpp/mm/pointers.hpp>
#include <cstdlib>
#include <cstdio>

using namespace sfcpp::arch;

// mem::sptr

namespace mem {

sptr::sptr(void* data): sptr(data, free) {};

void sptr::release() noexcept {
    if (!owner)
        return;
    if (--owner->references == 0) {
        owner->freer(owner->original);
        free(owner);
    }
    owner = nullptr;
    data = nullptr;
}

sptr::sptr(void* data, void(*freer)(void* data)): data(data) {
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
}

sptr::sptr(const sptr& other) noexcept : data(other.data), owner(other.owner) {
    if (owner)
        owner->references++;
}

sptr::sptr(sptr&& other) noexcept : data(other.data), owner(other.owner) {
    other.data = NULL;
    other.owner = NULL;
};

sptr& sptr::operator=(const sptr& other) noexcept {
    if (this == &other)
        return *this;

    release();

    data = other.data;
    owner = other.owner;

    if (owner)
        ++owner->references;

    return *this;
}

sptr& sptr::operator=(sptr&& other) noexcept {
    if (this == &other)
        return *this;

    release();

    data = other.data;
    owner = other.owner;

    other.data = NULL;
    other.owner = NULL;

    return *this;
}

sptr::~sptr() {
    release();
}

}
