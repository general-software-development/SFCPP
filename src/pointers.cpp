#include <sfcpp/arch/iint.hpp>
#include <sfcpp/mm/pointers.hpp>
#include <cstdlib>

using namespace sfcpp::arch;

void standard_ptr_freer(void* data) {
    free(data);
}

namespace mem {

ptr::ptr(void* data) {
    this->data = data;
    this->freer = standard_ptr_freer;
}

ptr::ptr(void* data, void(*freer)(void*)) {
    this->data = data;
    this->freer = freer;
}

ptr ptr::operator+(const u64& other) const {
    return ptr((char*)this->data + other);
}

ptr ptr::operator+(ptr& other) const {
    return ptr((char*)this->data + (u64)other.get_data());
}

ptr ptr::operator-(const u64& other) const {
    return ptr((char*)this->data - other);
}

ptr ptr::operator-(ptr& other) const {
    return ptr((char*)this->data - (u64)other.get_data());
}

}
