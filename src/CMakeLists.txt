cmake_minimum_required(VERSION 3.0)

project(singleton-cpp-lib C CXX)
enable_language(C CXX)

set(my_sources
    ../include/singleton-cpp/singleton.h
    ../include/singleton-cpp/singleton_api.h
    ./singleton.cpp
)

add_library(singleton SHARED ${my_sources})
target_include_directories(singleton PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# The CXX_EXTENSIONS property is set to OFF to disable compiler specific extensions
# (for example, on GCC this will make CMake use -std=c++17 rather than -std=gnu++17 in the list of compilation flags).
set_target_properties(singleton PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
)
