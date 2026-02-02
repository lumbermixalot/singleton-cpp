cmake_minimum_required(VERSION 4.0)

# PART 1/2. See PART 2/2 several lines below
# Because EnTT is a headers only library we need the following trick
# to make its header show up in the Solution Explorer.
if (MSVC)
    # Get EnTT include directory
    get_target_property(ENTT_INCLUDE_DIR_RAW EnTT::EnTT INTERFACE_INCLUDE_DIRECTORIES)
    
    # Evaluate the generator expression to get the actual path
    # For BUILD_INTERFACE, we can use file(GENERATE) or extract manually
    string(REGEX REPLACE "\\$<BUILD_INTERFACE:([^>]+)>.*" "\\1" ENTT_INCLUDE_DIR "${ENTT_INCLUDE_DIR_RAW}")

    # Find all EnTT headers
    file(GLOB_RECURSE ENTT_HEADERS "${ENTT_INCLUDE_DIR}/*.h*")

    # Organize in Solution Explorer
    source_group(TREE "${ENTT_INCLUDE_DIR}" FILES ${ENTT_HEADERS})   
endif()

project(singleton-cpp-lib C CXX)
enable_language(C CXX)

set(my_sources
    ../include/singleton-cpp/singleton.h
    ../include/singleton-cpp/singleton_api.h
    ./singleton.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    add_library(singleton STATIC ${my_sources})
else()
    add_library(singleton SHARED ${my_sources})
endif()
target_include_directories(singleton PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_sources(singleton PRIVATE 
    ${ENTT_HEADERS} # PART 2/2. This is essential to get "entt/" headers listed in visual studio solution. 
)

target_link_libraries(singleton PRIVATE 
    EnTT::EnTT
)

target_compile_definitions(singleton PRIVATE ENTT_ID_TYPE=std::uint64_t ENTT_USE_ATOMIC)

# The CXX_EXTENSIONS property is set to OFF to disable compiler specific extensions
# (for example, on GCC this will make CMake use -std=c++17 rather than -std=gnu++17 in the list of compilation flags).
set_target_properties(singleton PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
    CXX_EXCEPTIONS OFF
    CXX_RTTI OFF
)
