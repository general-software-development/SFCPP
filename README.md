# Simple & Fast C++

**SFCPP** is a library that attempts to bring Rust's safety and speed over to C++.

## Compilation

Requirements:

* CMake 3.16
* A C++20 or newer compiler

```bash
$ cmake .
-- Building for: Ninja
-- The CXX compiler identification is GNU 16.2.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Obscured/c++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.6s)
-- Generating done (0.0s)
-- Build files have been written to: E:/SFCPP
$ ninja
[5/5] Linking CXX static library libsfcpp.a
CF        libsfcpp.a
LD        libsfcpp.a
```

## Roadmap

* Arrays
  * [X] `mem::static_bounded_buffer<u64, T>`
    * [ ] Replace underlying storage from `void*` to `mem::sptr`
  * [ ] `mem::bounded_buffer<T>`
* Pointers
  * [X] `mem::ptr`
  * [ ] `mem::sptr` <!-- Arithmetic unimplemented -->
  * [ ] `mem::borrowed_ptr`
    * [ ] `mem::sptr.borrow()`
  * [ ] `mem::unsafe_borrowed_ptr`  <!-- .as() returns references to objects instead of copying them, compared to borrowed_ptr -->
* Memory Allocation
  * [ ] Arena implementation
  * [ ] Arena heap implementation
  * [ ] Memory allocation
  * [ ] Bulk memory allocation
  * [ ] Memory deallocation
