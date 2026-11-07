// SFCPP public API test suite (C++20)
// Compile against your project's libsfcpp.a; implementation .cpp files
// were not present in the uploaded archive.
//
// Optional features (marked TODO in the uploaded header):
//   -DSFCPP_TEST_ARITHMETIC
//   -DSFCPP_TEST_POINTER_OPERANDS (also requires SFCPP_TEST_ARITHMETIC)
//
// Pointer-operand tests assume the RHS points to a u64 byte offset.

#include <cstddef> // pointers.hpp currently uses NULL without including <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <type_traits>
#include <utility>
#include <vector>

#include <sfcpp/arch/iint.hpp>
#include <sfcpp/mm/pointers.hpp>
#include <sfcpp/mm/buffers.hpp>

namespace {
int checks_passed = 0;
int checks_failed = 0;
int int_deletions = 0;
int byte_deletions = 0;
int offset_deletions = 0;

#define CHECK(expr)                                                        \
    do {                                                                   \
        if (expr) {                                                        \
            ++checks_passed;                                               \
        } else {                                                           \
            ++checks_failed;                                               \
            std::printf("  FAIL at line %d: %s\n", __LINE__, #expr);      \
        }                                                                  \
    } while (false)

void delete_int(void* p) {
    ++int_deletions;
    delete static_cast<int*>(p);
}

[[maybe_unused]] void free_bytes(void* p) {
    ++byte_deletions;
    std::free(p);
}

[[maybe_unused]] void delete_offset(void* p) {
    ++offset_deletions;
    delete static_cast<sfcpp::arch::u64*>(p);
}

void run(const char* name, void (*test)()) {
    std::printf("[TEST] %s\n", name);
    try {
        test();
    } catch (const std::exception& e) {
        ++checks_failed;
        std::printf("  EXCEPTION: %s\n", e.what());
    } catch (...) {
        ++checks_failed;
        std::printf("  UNKNOWN EXCEPTION\n");
    }
}

void test_integer_types() {
    using namespace sfcpp::arch;
    static_assert(sizeof(i64) == 8);
    static_assert(sizeof(u64) == 8);
    static_assert(sizeof(u32) == 4);
    static_assert(sizeof(u8) == 1);
    static_assert(std::is_same_v<uchar, u8>);

    i64 signed_value = -10;
    u64 unsigned_value = 10;
    u32 word = 0x12345678;
    uchar ch = 65;
    CHECK(signed_value == -10);
    CHECK(unsigned_value == 10);
    CHECK(word == 0x12345678u);
    CHECK(ch == static_cast<uchar>('A'));
}

void test_ptr_default_deleter_and_access() {
    void* raw = std::malloc(sizeof(int));
    CHECK(raw != nullptr);
    if (!raw) return;

    // int is an implicit-lifetime type under C++20.
    *static_cast<int*>(raw) = 42;
    mem::ptr<int> p(raw);
    CHECK(p.get_data() == raw);
    CHECK(*p == 42);
    *p = 99;
    const mem::ptr<int>& cp = p;
    CHECK(*cp() == 99);
    p.free(); // Explicit: mem::ptr has no user-declared destructor.
}

void test_ptr_custom_deleter() {
    int before = int_deletions;
    {
        mem::ptr<int> p(new int(21), delete_int);
        CHECK(*p == 21);
        *p = 22;
        CHECK(*p == 22);
        p.free();
        CHECK(int_deletions == before + 1);
    }
    CHECK(int_deletions == before + 1); // No automatic second deletion.
}

void test_sptr_copy_construction() {
    int before = int_deletions;
    {
        mem::sptr<int> a(new int(42), delete_int);
        void* raw = a.get_data();
        CHECK(*a == 42);
        {
            mem::sptr b(a);
            mem::sptr c = b;
            CHECK(a.get_data() == raw);
            CHECK(b.get_data() == raw);
            CHECK(c.get_data() == raw);
            *b = 100;
            CHECK(*a == 100);
            CHECK(int_deletions == before);
        }
        CHECK(int_deletions == before);
        CHECK(*a == 100);
    }
    CHECK(int_deletions == before + 1);
}

void test_sptr_move_construction() {
    int before = int_deletions;
    {
        mem::sptr a(new int(17), delete_int);
        void* raw = a.get_data();
        mem::sptr b(std::move(a));
        CHECK(b.get_data() == raw);
        CHECK(b.as<int>() == 17);
        CHECK(a.get_data() == nullptr);
        CHECK(int_deletions == before);
    }
    CHECK(int_deletions == before + 1);
}

void test_sptr_copy_assignment() {
    int before = int_deletions;
    {
        mem::sptr a(new int(1), delete_int);
        mem::sptr b(new int(2), delete_int);
        b = a;
        CHECK(int_deletions == before + 1); // Previous b allocation freed.
        CHECK(b.get_data() == a.get_data());
        CHECK(b.as<int>() == 1);
        a.as<int>() = 5;
        CHECK(b.as<int>() == 5);
        b = b; // Self-copy assignment should be safe.
        CHECK(b.as<int>() == 5);
    }
    CHECK(int_deletions == before + 2);
}

void test_sptr_move_assignment() {
    int before = int_deletions;
    {
        mem::sptr a(new int(11), delete_int);
        mem::sptr b(new int(22), delete_int);
        void* raw = a.get_data();
        b = std::move(a);
        CHECK(int_deletions == before + 1); // Old b was freed.
        CHECK(a.get_data() == nullptr);
        CHECK(b.get_data() == raw);
        CHECK(b.as<int>() == 11);
        mem::sptr& same_object = b;
        b = std::move(same_object); // Self-move assignment should be safe.
        CHECK(b.as<int>() == 11);
    }
    CHECK(int_deletions == before + 2);
}

void test_sptr_many_owners() {
    int before = int_deletions;
    {
        mem::sptr base(new int(123), delete_int);
        {
            std::vector<mem::sptr> owners;
            owners.reserve(128);
            for (int i = 0; i < 128; ++i)
                owners.push_back(base);
            CHECK(owners.size() == 128);
            CHECK(owners[127].as<int>() == 123);
            CHECK(int_deletions == before);
        }
        CHECK(int_deletions == before);
    }
    CHECK(int_deletions == before + 1);
}

void test_sptr_default_deleter() {
    void* raw = std::malloc(sizeof(int));
    CHECK(raw != nullptr);
    if (!raw) return;
    *static_cast<int*>(raw) = 88;
    {
        mem::sptr p(raw); // Must deallocate with std::free.
        CHECK(p.get_data() == raw);
        CHECK(p.as<int>() == 88);
    }
    // Run under ASan/Valgrind to detect leaks or invalid frees.
}

void test_buffer_constructor_and_indexing() {
    int storage[64] = {};
    for (int i = 0; i < 64; ++i)
        storage[i] = i * 3;

    mem::static_bounded_buffer<64, int> buffer(storage);
    CHECK(buffer.get_data() == storage);
    CHECK(buffer[0] == 0);
    CHECK(buffer[1] == 3);
    CHECK(buffer[63] == 189);
    buffer[5] = 555;
    CHECK(storage[5] == 555);
    const auto& read_only = buffer;
    CHECK(read_only[5] == 555);
    CHECK(read_only[63] == 189);
}

void test_buffer_default_constructor() {
    // Construction only: the uploaded header doesn't define what its
    // default constructor initializes, so don't dereference it blindly.
    mem::static_bounded_buffer<64, int> buffer;
    (void)buffer;
    CHECK(buffer.get_data() != NULL);
    free(buffer.get_data());
}

#ifdef SFCPP_TEST_ARITHMETIC
void test_ptr_numeric_arithmetic() {
    int before = byte_deletions;
    auto* raw = static_cast<unsigned char*>(std::malloc(64));
    CHECK(raw != nullptr);
    if (!raw) return;
    {
        mem::ptr base(raw, free_bytes);
        mem::ptr p8 = base + sfcpp::arch::u64{8};
        mem::ptr p16 = p8 + sfcpp::arch::u64{8};
        mem::ptr back = p16 - sfcpp::arch::u64{16};
        CHECK(p8.get_data() == raw + 8);
        CHECK(p16.get_data() == raw + 16);
        CHECK(back.get_data() == raw);
        base.free(); // Do not free offset pointers.
    }
    CHECK(byte_deletions == before + 1);
}

void test_sptr_numeric_arithmetic() {
    int before = byte_deletions;
    auto* raw = static_cast<unsigned char*>(std::malloc(64));
    CHECK(raw != nullptr);
    if (!raw) return;
    {
        mem::sptr base(raw, free_bytes);
        {
            mem::sptr p8 = base + sfcpp::arch::u64{8};
            mem::sptr p16 = p8 + sfcpp::arch::u64{8};
            mem::sptr back = p16 - sfcpp::arch::u64{16};
            CHECK(p8.get_data() == raw + 8);
            CHECK(p16.get_data() == raw + 16);
            CHECK(back.get_data() == raw);
            CHECK(byte_deletions == before);
        }
        CHECK(byte_deletions == before);
    }
    CHECK(byte_deletions == before + 1);
}
#endif

#ifdef SFCPP_TEST_POINTER_OPERANDS
void test_ptr_pointer_arithmetic() {
    // Assumes the RHS pointer points to a u64 BYTE OFFSET.
    auto* raw = static_cast<unsigned char*>(std::malloc(64));
    CHECK(raw != nullptr);
    if (!raw) return;
    mem::ptr base(raw, std::free);
    mem::ptr offset(new sfcpp::arch::u64(8), delete_offset);
    mem::ptr added = base + offset;
    mem::ptr subtracted = added - offset;
    CHECK(added.get_data() == raw + 8);
    CHECK(subtracted.get_data() == raw);
    offset.free();
    base.free();
}

void test_sptr_pointer_arithmetic() {
    // Assumes the RHS pointer points to a u64 BYTE OFFSET.
    int before = byte_deletions;
    auto* raw = static_cast<unsigned char*>(std::malloc(64));
    CHECK(raw != nullptr);
    if (!raw) return;
    {
        mem::sptr base(raw, free_bytes);
        mem::sptr offset(new sfcpp::arch::u64(8), delete_offset);
        mem::sptr added = base + offset;
        mem::sptr subtracted = added - offset;
        CHECK(added.get_data() == raw + 8);
        CHECK(subtracted.get_data() == raw);
    }
    CHECK(byte_deletions == before + 1);
}
#endif
} // namespace

int main() {
    run("integer type aliases", test_integer_types);
    run("ptr: raw constructor, get_data, mutable/const as, free", test_ptr_default_deleter_and_access);
    run("ptr: custom deleter", test_ptr_custom_deleter);
    run("sptr: copy constructor and shared access", test_sptr_copy_construction);
    run("sptr: move constructor", test_sptr_move_construction);
    run("sptr: copy assignment and self-assignment", test_sptr_copy_assignment);
    run("sptr: move assignment and self-move", test_sptr_move_assignment);
    run("sptr: reference counting with 128 owners", test_sptr_many_owners);
    run("sptr: default deleter", test_sptr_default_deleter);
    run("static_bounded_buffer: pointer constructor, get_data, []", test_buffer_constructor_and_indexing);
    run("static_bounded_buffer: default constructor", test_buffer_default_constructor);
#ifdef SFCPP_TEST_ARITHMETIC
    run("ptr: numeric + and -", test_ptr_numeric_arithmetic);
    run("sptr: numeric + and -", test_sptr_numeric_arithmetic);
#else
    std::puts("[SKIP] arithmetic operators (marked TODO in uploaded headers)");
#endif
#ifdef SFCPP_TEST_POINTER_OPERANDS
    run("ptr: ptr + ptr and ptr - ptr", test_ptr_pointer_arithmetic);
    run("sptr: sptr + sptr and sptr - sptr", test_sptr_pointer_arithmetic);
#else
    std::puts("[SKIP] pointer-operand arithmetic (semantics unconfirmed)");
#endif
    std::printf("\nRESULT: %d passed, %d failed\n", checks_passed, checks_failed);
    return checks_failed == 0 ? 0 : 1;
}
