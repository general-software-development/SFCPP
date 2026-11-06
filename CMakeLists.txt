cmake_minimum_required(VERSION 3.16)

project(sfcpp LANGUAGES CXX)

add_library(sfcpp STATIC
    src/arrays.cpp
)

target_compile_features(sfcpp PUBLIC cxx_std_17)

set_target_properties(sfcpp PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_EXTENSIONS ON
)

target_include_directories(sfcpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_custom_command(TARGET sfcpp PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E echo
        "LD        $<TARGET_FILE_NAME:sfcpp>"
    VERBATIM
)

add_custom_command(TARGET sfcpp PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
        "CF        $<TARGET_FILE_NAME:sfcpp>"
    VERBATIM
)