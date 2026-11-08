# Safe & Fast C++

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

## Roadmap (11/23)

![Completion](https://img.shields.io/badge/Completion-47%25-orange?style=for-the-badge)

* Arrays (4/5)
  * [X] `mem::static_bounded_buffer<u64, T>`
    * [X] Replace underlying storage from `void*` to `mem::sptr`
    * [X] Make members inlined
  * [X] `mem::bounded_buffer<T>`
  * [ ] `mem::unbounded_buffer<T>`
* Pointers (6/9)
  * [X] `mem::ptr`
    * [X] Pointer arithmetic
    * [X] Templated class, with inlined functions
  * [X] `mem::sptr<T>`
    * [X] Pointer arithmetic
    * [X] Templated class, with inlined functions (!!)
  * [ ] `mem::borrowed_ptr<T>`
    * [ ] `mem::sptr.borrow()`
  * [ ] `mem::unsafe_borrowed_ptr`  <!-- .as() returns references to objects instead of copying them, compared to borrowed_ptr -->
* Memory Allocation (1/5)
  * [X] Arena implementation
  * [ ] Arena heap implementation
  * [ ] Memory allocation
  * [ ] Bulk memory allocation
  * [ ] Memory deallocation
* Thread Safety (0/4)
  * [ ] ~~Lock-based `mem::borrowed_ptr` writes~~
  * [ ] ~~Lock-based `mem::sptr` `.as()` calls~~
  * [ ] Use locks in `mem::sptr` control structs, when updating reference counting
  * [ ] Optional `mem::sptr` locking
    * [ ] `mem::sptr.get_lock()`
    * [ ] `mem::sptr.release_lock()`

<svg xmlns="http://www.w3.org/2000/svg" width="690" height="24">
  <rect width="690" height="24" rx="1" fill="#30363d"/>
  <rect width="330" height="24" rx="1" fill="#238636"/>
  <text x="345" y="16" text-anchor="middle"
        fill="white" font-size="13" font-family="Courier">
    47.82%
  </text>
</svg>
