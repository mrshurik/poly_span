cmake_minimum_required(VERSION 3.1)

cmake_policy(SET CMP0057 NEW)

project(PolySpan)

enable_testing()

add_library(poly_span INTERFACE)

target_sources(poly_span INTERFACE ${CMAKE_SOURCE_DIR}/include/span/poly_span.h)

target_include_directories(poly_span INTERFACE include)

add_subdirectory(tests)
