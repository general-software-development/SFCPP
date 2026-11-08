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
[3/3] Linking CXX static library libsfcpp.a
CF        libsfcpp.a
LD        libsfcpp.a
```

## Roadmap (11/24)

![Progress](./Assets/progress.svg)

* Arrays (4/6)
  * [X] `mem::static_bounded_buffer<u64, T>`
    * [X] Replace underlying storage from `void*` to `mem::sptr`
    * [X] Make members inlined
    * [ ] Moves, copies, get_size()
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

<br>
<p align=center>&copy; Copyright 2026, Licensed under the <b>MIT License.</b></p>
