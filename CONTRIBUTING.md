# Contributing

## 1. Use of standard library

1. Prefer using `uchar*` over `std::string`
2. Use `const uchar* const` over `std::string_view`
3. Avoid overuse of the C++ STL

## 2. AI-Generated Contributions

1. If a PR or Issue is generated with the use of AI, please explicitly and clearly disclose what parts of it were generated with AI and what AI was used for.
2. PP descriptions and Issue contents are not to be generated with the use of AI. They will be closed without review.
3. Any functions generated entirely with the use of AI are to be marked as such with a comment (`// AI-Generated`) at the top of the function

## 3. C-Style C++

1. Prefer `uchar*` over `char*`
2. Avoid excessive use of `std::cout` or `std::cerr`. When printing is needed, use `printf()`

## 4. Code Maintenance

1. You are expected to help maintain the code you submit, if you submit large portions of code.
2. If you submit large portions of code, feel free to add yourself to CODEOWNERS. Note that this does **not** give you write access.

## 5. Comments, Documentation, and Readability

1. Write comments in your code
2. If your code is very complex, include a markdown file under a `docs` folder in the root directory that documents it
3. Do not intentionally obscure or "minimise" your code
4. When declaring one pointer, put the asterisk (`*`) next to the type. Ex: `int* values`
5. When declaring multiple pointers, put the asterisks (`*`) next to the variable names. Ex: `void *src, *dest`

## 6. Reproducibility

1. Do not write architecture-specific code
2. Do not write platform-specific code
3. Do not write compiler-specific code
