cmake_minimum_required(VERSION 3.16)

project(sfcpp LANGUAGES CXX)

add_library(sfcpp_mm OBJECT
    src/arrays.cpp
)

add_library(sfcpp_internal OBJECT
    src/internal.cpp
)

function(configure_sfcpp_target target)
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_compile_features(${target} INTERFACE cxx_std_17)

    set_target_properties(${target} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
    )

    target_include_directories(${target} PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}
    )

    target_compile_options(${target} PRIVATE -fpermissive)
endfunction()

configure_sfcpp_target(sfcpp_mm)

configure_sfcpp_target(sfcpp_internal)

add_library(sfcpp STATIC)

target_link_libraries(sfcpp PRIVATE
    sfcpp_mm
    sfcpp_internal
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
